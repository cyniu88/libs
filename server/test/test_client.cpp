#include <gtest/gtest.h>
#include "../client.hpp"
#include "../crypto.hpp"
#include <thread>
#include <chrono>

class ClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_key = Crypto::generateKey();
        client = std::make_unique<TCPClient>("127.0.0.1", 12345, test_key);
    }
    
    void TearDown() override {
        if (client && client->isConnected()) {
            client->disconnect();
        }
    }
    
    std::unique_ptr<TCPClient> client;
    std::string test_key;
};

TEST_F(ClientTest, InitialStateNotConnected) {
    EXPECT_FALSE(client->isConnected());
}

TEST_F(ClientTest, InvalidAddressThrowsException) {
    TCPClient invalid_client("invalid.address", 12345, test_key);
    EXPECT_THROW(invalid_client.connect(), std::runtime_error);
}

TEST_F(ClientTest, ConnectToNonExistentServerThrowsException) {
    TCPClient test_client("127.0.0.1", 99999, test_key);
    EXPECT_THROW(test_client.connect(), std::runtime_error);
}

TEST_F(ClientTest, SendWithoutConnectionReturnsFalse) {
    EXPECT_FALSE(client->send("test message"));
}

TEST_F(ClientTest, ReceiveWithoutConnectionReturnsEmpty) {
    EXPECT_EQ(client->receive(), "");
}

TEST_F(ClientTest, DisconnectWhenNotConnectedDoesNotCrash) {
    EXPECT_NO_THROW(client->disconnect());
    EXPECT_FALSE(client->isConnected());
}