#include <gtest/gtest.h>
#include "../server.hpp"
#include "../crypto.hpp"
#include <thread>
#include <chrono>

class ServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_key = Crypto::generateKey();
        server = std::make_unique<TCPServer>(12346, test_key);
    }
    
    void TearDown() override {
        if (server) {
            server->stop();
        }
    }
    
    std::unique_ptr<TCPServer> server;
    std::string test_key;
};

TEST_F(ServerTest, ServerCreationDoesNotThrow) {
    std::string key = Crypto::generateKey();
    EXPECT_NO_THROW(TCPServer test_server(12347, key));
}

TEST_F(ServerTest, ServerStartAndStop) {
    EXPECT_NO_THROW(server->start());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NO_THROW(server->stop());
}

TEST_F(ServerTest, InvalidPortThrowsException) {
    // Port 1 requires root privileges, should throw exception
    TCPServer invalid_server(1, test_key);
    EXPECT_THROW(invalid_server.start(), std::runtime_error);
}

TEST_F(ServerTest, PortAlreadyInUseThrowsException) {
    server->start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    TCPServer duplicate_server(12346, test_key);
    EXPECT_THROW(duplicate_server.start(), std::runtime_error);
}

TEST_F(ServerTest, BroadcastWithoutClientsDoesNotCrash) {
    server->start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    EXPECT_NO_THROW(server->broadcast("test message"));
}

TEST_F(ServerTest, StopWithoutStartDoesNotCrash) {
    EXPECT_NO_THROW(server->stop());
}