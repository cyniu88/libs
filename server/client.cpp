#include "client.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>

TCPClient::TCPClient(const std::string& ip, int port, const std::string& encryption_key)
    : sock(-1), 
      ip_address(ip), 
      port(port),
      crypto(encryption_key) {}

TCPClient::~TCPClient() {
    disconnect();
}

void TCPClient::connect() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Socket creation failed");
    }

    // Set non-blocking mode
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address");
    }

    while (::connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        if (errno == EINPROGRESS || errno == EAGAIN || errno == EINTR) {
            // Wait for connection
            struct pollfd pfd = {sock, POLLOUT, 0};
            int ret = poll(&pfd, 1, 1000);  // 1 second timeout
            
            if (ret > 0 && (pfd.revents & POLLOUT)) {
                // Check if connection succeeded
                int error = 0;
                socklen_t len = sizeof(error);
                if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
                    throw std::runtime_error("Connection failed");
                }
                break;
            }
        } else {
            throw std::runtime_error("Connection failed");
        }
    }

    connected = true;
    pfd.fd = sock;
    pfd.events = POLLIN | POLLOUT;
}

void TCPClient::disconnect() {
    // set flag first
    connected.store(false);

    // interrupt blocking recv/send by shutting down the socket
    {
        std::lock_guard<std::mutex> lock(sock_mutex);
        if (sock >= 0) {
            ::shutdown(sock, SHUT_RDWR); // causes blocking recv to return
        }
    }

    // now close socket
    {
        std::lock_guard<std::mutex> lock(sock_mutex);
        if (sock >= 0) {
            ::close(sock);
            sock = -1;
        }
    }
}

bool TCPClient::send(const std::string& message) {
    std::lock_guard<std::mutex> lock(sock_mutex);
    if (!connected || sock < 0) return false;

    try {
        std::string encrypted = crypto.encrypt(message);
        size_t total_sent = 0;
        const size_t msg_len = encrypted.length();

        while (total_sent < msg_len) {
            ssize_t sent = ::send(sock, 
                                encrypted.data() + total_sent, 
                                msg_len - total_sent, 
                                MSG_NOSIGNAL);
            
            if (sent < 0) {
                if (errno == EINTR || errno == EAGAIN) continue;
                std::cerr << "Błąd wysyłania: " << strerror(errno) << std::endl;
                connected = false;
                return false;
            }
            total_sent += sent;
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Błąd szyfrowania: " << e.what() << std::endl;
        return false;
    }
}

std::string TCPClient::receive() {
    std::lock_guard<std::mutex> lock(sock_mutex);
    if (!connected || sock < 0) return "";

    try {
        char buffer[1024] = {0};
        ssize_t received = ::recv(sock, buffer, sizeof(buffer) - 1, 0);
        
        if (received > 0) {
            std::string encrypted(buffer, received);
            return crypto.decrypt(encrypted);
        } else if (received == 0) {
            // Serwer zamknął połączenie
            connected = false;
        } else if (received < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Błąd odbioru: " << strerror(errno) << std::endl;
            connected = false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Błąd deszyfrowania: " << e.what() << std::endl;
    }
    return "";
}