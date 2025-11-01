#include "client.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <errno.h>

TCPClient::TCPClient(const std::string& ip, int port)
    : sock(-1), ip_address(ip), port(port) {}

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

std::string TCPClient::receive() {
    if (!connected || sock < 0) {
        return "";
    }

    char buffer[1024] = {0};
    ssize_t received = ::recv(sock, buffer, sizeof(buffer) - 1, 0);
    
    if (received > 0) {
        return std::string(buffer, received);
    } else if (received == 0 || (received < 0 && errno != EAGAIN)) {
        connected = false;
    }
    return "";
}

bool TCPClient::send(const std::string& message) {
    if (!connected || sock < 0) {
        return false;
    }

    size_t total_sent = 0;
    const char* buffer = message.c_str();
    size_t length = message.length();

    while (total_sent < length) {
        ssize_t sent = ::send(sock, buffer + total_sent, length - total_sent, MSG_NOSIGNAL);
        if (sent < 0) {
            if (errno == EINTR) continue;
            std::cerr << "Send error: " << strerror(errno) << std::endl;
            connected = false;
            return false;
        }
        total_sent += sent;
    }
    return true;
}