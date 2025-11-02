#include "crypto.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

Crypto::Crypto(const std::string& hex_key) {
    if (sodium_init() < 0) {
        throw std::runtime_error("libsodium initialization failed");
    }
    
    if (!isValidHexKey(hex_key)) {
        throw std::runtime_error("Invalid hex key format or length");
    }
    
    encryption_key = hexToBytes(hex_key);
}

std::string Crypto::encrypt(const std::string& message) {
    std::vector<unsigned char> ciphertext(message.length() + crypto_aead_chacha20poly1305_ABYTES);
    std::vector<unsigned char> nonce(crypto_aead_chacha20poly1305_NPUBBYTES);
    randombytes_buf(nonce.data(), nonce.size());
    
    unsigned long long ciphertext_len;
    
    if (crypto_aead_chacha20poly1305_encrypt(
            ciphertext.data(), &ciphertext_len,
            reinterpret_cast<const unsigned char*>(message.data()),
            message.length(),
            nullptr, 0,
            nullptr,
            nonce.data(),
            encryption_key.data()) != 0) {
        throw std::runtime_error("Encryption failed");
    }
    
    std::string result;
    result.reserve(nonce.size() + ciphertext_len);
    result.append(reinterpret_cast<char*>(nonce.data()), nonce.size());
    result.append(reinterpret_cast<char*>(ciphertext.data()), ciphertext_len);
    
    return result;
}

std::string Crypto::decrypt(const std::string& encrypted_message) {
    if (encrypted_message.length() < crypto_aead_chacha20poly1305_NPUBBYTES + crypto_aead_chacha20poly1305_ABYTES) {
        throw std::runtime_error("Message too short");
    }
    
    const unsigned char* nonce = reinterpret_cast<const unsigned char*>(encrypted_message.data());
    const unsigned char* ciphertext = reinterpret_cast<const unsigned char*>(
        encrypted_message.data() + crypto_aead_chacha20poly1305_NPUBBYTES);
    size_t ciphertext_len = encrypted_message.length() - crypto_aead_chacha20poly1305_NPUBBYTES;
    
    std::vector<unsigned char> decrypted(ciphertext_len - crypto_aead_chacha20poly1305_ABYTES);
    unsigned long long decrypted_len;
    
    if (crypto_aead_chacha20poly1305_decrypt(
            decrypted.data(), &decrypted_len,
            nullptr,
            ciphertext, ciphertext_len,
            nullptr, 0,
            nonce,
            encryption_key.data()) != 0) {
        throw std::runtime_error("Decryption failed");
    }
    
    return std::string(reinterpret_cast<char*>(decrypted.data()), decrypted_len);
}

std::string Crypto::generateKey() {
    std::vector<unsigned char> key(KEY_LENGTH);
    crypto_aead_chacha20poly1305_keygen(key.data());
    return bytesToHex(key);
}

bool Crypto::isValidHexKey(const std::string& hex_key) {
    if (hex_key.length() != KEY_LENGTH * 2) return false;
    return hex_key.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
}

std::string Crypto::bytesToHex(const std::vector<unsigned char>& bytes) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned char byte : bytes) {
        ss << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}

std::vector<unsigned char> Crypto::hexToBytes(const std::string& hex) {
    std::vector<unsigned char> bytes;
    bytes.reserve(hex.length() / 2);
    
    for (size_t i = 0; i < hex.length(); i += 2) {
        unsigned int byte;
        std::istringstream(hex.substr(i, 2)) >> std::hex >> byte;
        bytes.push_back(static_cast<unsigned char>(byte));
    }
    
    return bytes;
}