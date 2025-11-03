#pragma once
#include <string>
#include <atomic>
#include <mutex>
#include <sys/socket.h>
#include <poll.h>
#include "crypto.hpp"

class TCPClient {
public:
    TCPClient(const std::string& ip, int port, const std::string& encryption_key);
    ~TCPClient();

    void connect();
    void disconnect();
    bool send(const std::string& message);
    std::string receive();
    bool isConnected() const { return connected; }

private:
    int sock;
    std::string ip_address;
    int port;
    std::atomic<bool> connected{false};
    std::mutex sock_mutex;
    Crypto crypto;
    struct pollfd pfd;
};