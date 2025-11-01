#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <csignal>
#include "../client.hpp"

static std::atomic<bool> g_running{true};
static TCPClient* g_client = nullptr;

void sigint_handler(int) {
    g_running = false;
    if (g_client) {
        g_client->disconnect();
    }
}

int main(int argc, char** argv) {
    signal(SIGINT, sigint_handler);

    const std::string server_ip = (argc > 1) ? argv[1] : "127.0.0.1";
    const int server_port = (argc > 2) ? std::stoi(argv[2]) : 12345;

    try {
        TCPClient client(server_ip, server_port);
        g_client = &client;

        std::cout << "Connecting to " << server_ip << ":" << server_port << std::endl;
        client.connect();
        std::cout << "Connected!" << std::endl;

        // Reader thread
        std::thread reader([&client]() {
            while (g_running && client.isConnected()) {
                std::string msg = client.receive();
                if (!msg.empty()) {
                    std::cout << "\r[Server] " << msg << std::flush;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });

        // Main thread handles input
        std::string input;
        while (g_running && client.isConnected()) {
            if (std::getline(std::cin, input)) {
                if (input == "quit" || input == "exit") {
                    break;
                }
                
                if (!client.send(input + "\n")) {
                    std::cerr << "Failed to send message" << std::endl;
                    break;
                }
            }
        }

        g_running = false;
        client.disconnect();
        if (reader.joinable()) {
            reader.join();
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}