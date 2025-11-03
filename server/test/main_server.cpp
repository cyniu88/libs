#include "../server.hpp"
#include <csignal>
#include <atomic>
#include <iostream>

static std::atomic<bool> g_running{true};
static TCPServer *g_server = nullptr;

void sigint_handler(int)
{
    std::cout << "\nReceived shutdown signal..." << std::endl;
    g_running = false;
    if (g_server)
    {
        g_server->stop();
    }
}

int main()
{
    // Setup signal handler
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, nullptr);

    const int port = 12345;

    try
    {
        // Generate a proper key using libsodium
        //const std::string encryption_key = Crypto::generateKey();
        std::string encryption_key = "49478e0e901a6ce61dbc3864756c709a914e30f2534921323c127bcb7fcd2303";
        std::cout << "Generated key (save and share securely): " << encryption_key << std::endl;

        TCPServer server(port, encryption_key);
        g_server = &server;

        std::cout << "Starting server on port " << port << std::endl;
        server.start();

        // Main loop
        std::string input;
        while (g_running)
        {
            if (std::getline(std::cin, input))
            {
                if (input == "quit" || input == "exit")
                {
                    g_running = false;
                    break;
                }
                server.broadcast(input + "\n");
            }
        }

        std::cout << "Shutting down server..." << std::endl;
        server.stop();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}