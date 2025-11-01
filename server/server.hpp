#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class TCPServer {
public:
    static constexpr int MAX_CLIENTS = 10;
    
    TCPServer(int port);
    ~TCPServer();
    
    void start();
    void stop();
    void broadcast(const std::string& message);
    void sendTo(int client_id, const std::string& message);

private:
    void acceptClients();
    void handleClient(int client_socket, int client_id);
    
    int server_fd;
    int port;
    std::atomic<bool> running{false};
    std::thread acceptor_thread;
    
    struct ClientInfo {
        int socket;
        std::thread handler;
        bool active;
    };
    
    std::vector<ClientInfo> clients;
    std::mutex clients_mutex;
};