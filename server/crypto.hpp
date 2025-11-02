#pragma once
#include <string>
#include <vector>
#include <sodium.h>

class Crypto {
public:
    static constexpr size_t KEY_LENGTH = crypto_aead_chacha20poly1305_KEYBYTES;
    
    explicit Crypto(const std::string& hex_key);
    std::string encrypt(const std::string& message);
    std::string decrypt(const std::string& encrypted_message);
    
    static std::string generateKey();
    static bool isValidHexKey(const std::string& hex_key);

private:
    std::vector<unsigned char> encryption_key;
    
    static std::string bytesToHex(const std::vector<unsigned char>& bytes);
    static std::vector<unsigned char> hexToBytes(const std::string& hex);
};