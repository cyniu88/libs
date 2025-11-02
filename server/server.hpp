#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <queue>
#include "crypto.hpp"

class TCPServer {
public:
    static constexpr int MAX_CLIENTS = 10;
    
    TCPServer(int port, const std::string& encryption_key);
    ~TCPServer();
    
    void start();
    void stop();
    void broadcast(const std::string& message);

private:
    void acceptClients();
    void handleClient(int client_socket, int client_id);
    void processMessages();
    
    int server_fd;
    int port;
    std::atomic<bool> running{false};
    std::thread acceptor_thread;
    std::thread message_processor;
    
    struct ClientInfo {
        int socket;
        std::thread handler;
        bool active;
    };
    
    std::vector<ClientInfo> clients;
    std::mutex clients_mutex;
    std::queue<std::string> message_queue;
    std::mutex queue_mutex;
    Crypto crypto;
};