#include "server.hpp"
#include <cstring>
#include <iostream>

TCPServer::TCPServer(int port) : port(port), server_fd(-1) {
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
    std::cout << "Server started on port " << port << std::endl;
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
    
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_read > 0) {
            std::string message(buffer, bytes_read);
            
            // Tylko wyświetl wiadomość na serwerze, bez przekazywania dalej
            std::cout << "\rClient " << client_id << ": " << message << std::flush;
            
        } else if (bytes_read == 0) {
            std::cout << "\rClient " << client_id << " rozłączony" << std::endl;
            break;
        } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Błąd odczytu: " << strerror(errno) << std::endl;
            break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Cleanup połączenia
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        if (clients[client_id].active) {
            shutdown(client_socket, SHUT_RDWR);
            close(client_socket);
            clients[client_id].active = false;
            clients[client_id].socket = -1;
        }
    }
}

void TCPServer::broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (const auto& client : clients) {
        if (client.active) {
            ssize_t sent = send(client.socket, message.c_str(), message.length(), MSG_NOSIGNAL);
            if (sent < 0) {
                std::cerr << "Failed to send to client: " << strerror(errno) << std::endl;
            }
        }
    }
}

void TCPServer::stop() {
    if (!running) return;
    
    std::cout << "\nStopping server..." << std::endl;
    running = false;
    
    // First shutdown the listening socket
    if (server_fd >= 0) {
        shutdown(server_fd, SHUT_RDWR);
        close(server_fd);
        server_fd = -1;
    }
    
    // Then close all client connections and join threads
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        for (auto& client : clients) {
            if (client.active) {
                shutdown(client.socket, SHUT_RDWR);
                close(client.socket);
                client.active = false;
                client.socket = -1;
            }
            // Always join thread if joinable
            if (client.handler.joinable()) {
                client.handler.join();
            }
        }
    }
    
    if (acceptor_thread.joinable()) {
        acceptor_thread.join();
    }
    
    std::cout << "Server stopped." << std::endl;
}