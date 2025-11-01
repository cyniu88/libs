#pragma once
#include <string>
#include <atomic>
#include <mutex>
#include <iostream>

class TCPClient {
public:
    TCPClient(const std::string& ip, int port);
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
};