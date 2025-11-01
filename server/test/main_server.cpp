#include "../server.hpp"
#include <csignal>
#include <atomic>
#include <iostream>

static std::atomic<bool> g_running{true};
static TCPServer* g_server = nullptr;

void sigint_handler(int) {
    std::cout << "\nReceived shutdown signal..." << std::endl;
    g_running = false;
    if (g_server) {
        g_server->stop();
    }
}

int main() {
    // Setup signal handler
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, nullptr);
    
    const int port = 12345;
    
    try {
        TCPServer server(port);
        g_server = &server;
        
        std::cout << "Starting server on port " << port << std::endl;
        server.start();
        
        // Main loop
        std::string input;
        while (g_running && std::getline(std::cin, input)) {
            if (input == "quit" || input == "exit") {
                break;
            }
            server.broadcast(input + "\n");
        }
        
        server.stop();
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}