#include "server.hpp"
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

TCPServer::TCPServer(int port, const std::string& encryption_key) 
    : server_fd(-1), 
      port(port),
      crypto(encryption_key) {
    clients.resize(MAX_CLIENTS);
    for(auto& client : clients) {
        client.socket = -1;
        client.active = false;
    }
}

TCPServer::~TCPServer() {
    stop();
}

void TCPServer::start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        throw std::runtime_error("Socket creation failed");
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("setsockopt failed");
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        throw std::runtime_error("Listen failed");
    }

    running = true;
    acceptor_thread = std::thread(&TCPServer::acceptClients, this);
    message_processor = std::thread(&TCPServer::processMessages, this);
}

void TCPServer::acceptClients() {
    while (running) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        
        int new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (new_socket >= 0) {
            std::lock_guard<std::mutex> lock(clients_mutex);
            
            // Find a free slot or cleanup terminated threads
            int slot = -1;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (!clients[i].active) {
                    // If there's a terminated thread, join it before reusing the slot
                    if (clients[i].handler.joinable()) {
                        clients[i].handler.join();
                    }
                    slot = i;
                    break;
                }
            }

            if (slot >= 0) {
                clients[slot].socket = new_socket;
                clients[slot].active = true;
                // Move the thread to the new handler
                clients[slot].handler = std::thread(&TCPServer::handleClient, this, new_socket, slot);
                std::cout << "Client " << slot << " connected from " 
                         << inet_ntoa(client_addr.sin_addr) << std::endl;
            } else {
                // No free slots
                close(new_socket);
                std::cout << "Connection rejected - max clients reached" << std::endl;
            }
        }
    }
}

void TCPServer::handleClient(int client_socket, int client_id) {
    char buffer[1024];
    fd_set readfds;
    
    while (running) {
        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);
        
        struct timeval tv = {0, 10000}; // 10ms timeout
        int ready = select(client_socket + 1, &readfds, nullptr, nullptr, &tv);
        
        if (ready > 0) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_read > 0) {
                try {
                    std::string encrypted(buffer, bytes_read);
                    std::string decrypted = crypto.decrypt(encrypted);
                    std::cout << "Client " << client_id << ": " << decrypted;
                    std::cout.flush();
                } catch (const std::exception& e) {
                    std::cerr << "Decryption error: " << e.what() << std::endl;
                }
            } else if (bytes_read <= 0 && errno != EINTR) {
                break;
            }
        } else if (ready < 0 && errno != EINTR) {
            std::cerr << "Select error: " << strerror(errno) << std::endl;
            break;
        }

        if (!running) break;
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        if (clients[client_id].active) {
            shutdown(client_socket, SHUT_RDWR);
            close(client_socket);
            clients[client_id].active = false;
            clients[client_id].socket = -1;
            std::cout << "Client " << client_id << " disconnected" << std::endl;
        }
    }
}

void TCPServer::processMessages() {
    while (running) {
        std::string message;
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (!message_queue.empty()) {
                message = message_queue.front();
                message_queue.pop();
            }
        }
        
        if (!message.empty()) {
            std::cout << message << std::flush;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void TCPServer::broadcast(const std::string& message) {
    try {
        std::string encrypted = crypto.encrypt(message);
        std::lock_guard<std::mutex> lock(clients_mutex);
        
        for (const auto& client : clients) {
            if (client.active) {
                size_t total_sent = 0;
                const size_t msg_len = encrypted.length();
                
                while (total_sent < msg_len) {
                    const ssize_t sent = send(client.socket, 
                                      encrypted.c_str() + total_sent, 
                                      msg_len - total_sent, 
                                      MSG_NOSIGNAL);
                    
                    if (sent < 0) {
                        if (errno == EINTR) continue;
                        std::cerr << "Send error: " << strerror(errno) << std::endl;
                        break;
                    }
                    total_sent += static_cast<size_t>(sent);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Encryption error: " << e.what() << std::endl;
    }
}

void TCPServer::stop() {
    if (!running) return;
    
    running = false;
    
    if (server_fd >= 0) {
        shutdown(server_fd, SHUT_RDWR);
        close(server_fd);
    }
    
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        for (auto& client : clients) {
            if (client.active) {
                shutdown(client.socket, SHUT_RDWR);
                close(client.socket);
                client.active = false;
            }
            if (client.handler.joinable()) {
                client.handler.join();
            }
        }
    }
    
    if (acceptor_thread.joinable()) {
        acceptor_thread.join();
    }
    
    if (message_processor.joinable()) {
        message_processor.join();
    }
}