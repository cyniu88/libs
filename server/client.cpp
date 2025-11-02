#include "client.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>  // Add this include for std::cerr and std::endl
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
    std::lock_guard<std::mutex> lock(sock_mutex);
    if (connected.load()) return;

    sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) throw std::runtime_error("Socket creation failed");

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) <= 0) {
        ::close(sock);
        sock = -1;
        throw std::runtime_error("Invalid address");
    }

    if (::connect(sock, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        ::close(sock);
        sock = -1;
        throw std::runtime_error("Connection failed");
    }

    connected.store(true);
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
        ssize_t sent = ::send(sock, encrypted.c_str(), encrypted.length(), MSG_NOSIGNAL);
        return (sent == static_cast<ssize_t>(encrypted.length()));
    } catch (const std::exception& e) {
        std::cerr << "Encryption error: " << e.what() << std::endl;
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
        } else if (received == 0 || (received < 0 && errno != EINTR)) {
            connected = false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Decryption error: " << e.what() << std::endl;
    }
    return "";
}