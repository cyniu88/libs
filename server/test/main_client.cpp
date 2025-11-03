#include "../client.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <csignal>

static std::atomic<bool> g_running{true};
static TCPClient* g_client = nullptr;

void sigint_handler(int) {
    std::cout << "\nOtrzymano sygnał zakończenia..." << std::endl;
    g_running = false;
    if (g_client) {
        g_client->disconnect();
    }
}

int main(int argc, char** argv) {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, nullptr);

    const std::string server_ip = (argc > 1) ? argv[1] : "127.0.0.1";
    const int server_port = (argc > 2) ? std::stoi(argv[2]) : 12345;

    try {
        std::string encryption_key;
        std::cout << "Podaj klucz szyfrowania: ";
        std::getline(std::cin, encryption_key);

        TCPClient client(server_ip, server_port, encryption_key);
        g_client = &client;

        std::cout << "Łączenie z " << server_ip << ":" << server_port << std::endl;
        client.connect();
        std::cout << "Połączono!" << std::endl;

        // Wątek odbierający wiadomości
        std::thread reader([&client]() {
            while (g_running && client.isConnected()) {
                std::string msg = client.receive();
                if (!msg.empty()) {
                    std::cout << "\r[Serwer] " << msg << std::flush;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });

        // Główna pętla do wysyłania wiadomości
        std::string input;
        while (g_running && client.isConnected()) {
            if (std::getline(std::cin, input)) {
                if (input == "quit" || input == "exit") {
                    g_running = false;
                    break;
                }
                
                if (!client.send(input + "\n")) {
                    std::cerr << "Błąd wysyłania wiadomości" << std::endl;
                    break;
                }
            }
        }

        g_running = false;
        if (reader.joinable()) {
            reader.join();
        }
        client.disconnect();

    } catch (const std::exception& e) {
        std::cerr << "Błąd: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}