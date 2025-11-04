#include <gtest/gtest.h>
#include "../crypto.hpp"

class CryptoTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_key = Crypto::generateKey();
    }
    
    std::string test_key;
};

TEST_F(CryptoTest, GenerateKeyReturnsValidHex) {
    std::string key = Crypto::generateKey();
    EXPECT_EQ(key.length(), Crypto::KEY_LENGTH * 2);
    EXPECT_TRUE(Crypto::isValidHexKey(key));
}

TEST_F(CryptoTest, ValidHexKeyValidation) {
    EXPECT_TRUE(Crypto::isValidHexKey(test_key));
    EXPECT_FALSE(Crypto::isValidHexKey("invalid"));
    EXPECT_FALSE(Crypto::isValidHexKey(""));
    EXPECT_FALSE(Crypto::isValidHexKey("123g"));
}

TEST_F(CryptoTest, EncryptDecryptRoundTrip) {
    Crypto crypto(test_key);
    std::string message = "Hello, World!";
    
    std::string encrypted = crypto.encrypt(message);
    std::string decrypted = crypto.decrypt(encrypted);
    
    EXPECT_EQ(message, decrypted);
}

TEST_F(CryptoTest, EncryptProducesDifferentOutput) {
    Crypto crypto(test_key);
    std::string message = "test message";
    
    std::string encrypted1 = crypto.encrypt(message);
    std::string encrypted2 = crypto.encrypt(message);
    
    EXPECT_NE(encrypted1, encrypted2); // Different nonces
}

TEST_F(CryptoTest, InvalidKeyThrowsException) {
    EXPECT_THROW(Crypto crypto("invalid_key"), std::runtime_error);
}

TEST_F(CryptoTest, DecryptInvalidDataThrowsException) {
    Crypto crypto(test_key);
    EXPECT_THROW(crypto.decrypt("invalid_data"), std::runtime_error);
    EXPECT_THROW(crypto.decrypt(""), std::runtime_error);
}