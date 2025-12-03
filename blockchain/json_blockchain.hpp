#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <functional>
#include <fstream>
#include <iostream>

namespace JsonBlockchain
{
    class Block
    {
    public:
        std::string data;
        std::string hash;
        std::string prevHash;
        long long timestamp;
        int nonce;

        Block(const std::string &jsonData, const std::string &previousHash)
            : data(jsonData), prevHash(previousHash), nonce(0)
        {
            timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();
            hash = calculateHash();
        }

        std::string calculateHash() const
        {
            std::string input = prevHash + data + std::to_string(timestamp) + std::to_string(nonce);
            return sha256(input);
        }

        void mineBlock(int difficulty)
        {
            std::string target(difficulty, '0');
            int maxAttempts = 100000;
            int attempts = 0;

            while (hash.substr(0, difficulty) != target && attempts < maxAttempts)
            {
                nonce++;
                hash = calculateHash();
                attempts++;
            }
        }

    private:
        std::string sha256(const std::string &input) const
        {
            std::hash<std::string> hasher;
            size_t hashValue = hasher(input);
            std::stringstream ss;
            ss << std::hex << hashValue;
            return ss.str();
        }
    };

    class Blockchain
    {
    private:
        std::vector<Block> chain;
        int difficulty;

    public:
        Blockchain(int diff = 1) : difficulty(diff)
        {
            chain.emplace_back("{\"genesis\":true}", "0");
        }

        void addBlock(const std::string &jsonData)
        {
            Block newBlock(jsonData, getLatestBlock().hash);
            newBlock.mineBlock(difficulty);
            chain.push_back(newBlock);
        }

        Block &getLatestBlock()
        {
            return chain.back();
        }

        bool isChainValid() const
        {
            for (size_t i = 1; i < chain.size(); i++)
            {
                const Block &current = chain[i];
                const Block &previous = chain[i - 1];

                if (current.hash != current.calculateHash())
                {
                    std::cout << "Block " << i << " has invalid hash!\n";
                    std::cout << "Expected: " << current.calculateHash() << "\n";
                    std::cout << "Actual: " << current.hash << "\n";
                    return false;
                }

                if (current.prevHash != previous.hash)
                {
                    std::cout << "Block " << i << " has broken chain link!\n";
                    std::cout << "Expected prevHash: " << previous.hash << "\n";
                    std::cout << "Actual prevHash: " << current.prevHash << "\n";
                    return false;
                }
            }
            return true;
        }

        const std::vector<Block> &getChain() const { return chain; }

        std::string getBlockData(size_t index) const
        {
            return (index < chain.size()) ? chain[index].data : "";
        }

        size_t size() const { return chain.size(); }

        bool saveToFile(const std::string &filename) const
        {
            std::ofstream file(filename);
            if (!file.is_open())
                return false;

            file << chain.size() << "\n";
            for (const auto &block : chain)
            {
                file << block.data << "\n"
                     << block.hash << "\n"
                     << block.prevHash << "\n"
                     << block.timestamp << "\n"
                     << block.nonce << "\n";
            }
            return true;
        }

        bool loadFromFile(const std::string &filename)
        {
            std::ifstream file(filename);
            if (!file.is_open())
                return false;

            size_t chainSize;
            file >> chainSize;
            file.ignore();

            chain.clear();
            for (size_t i = 0; i < chainSize; i++)
            {
                std::string data, hash, prevHash;
                long long timestamp;
                int nonce;

                std::getline(file, data);
                std::getline(file, hash);
                std::getline(file, prevHash);
                file >> timestamp >> nonce;
                file.ignore();

                Block block(data, prevHash);
                block.hash = hash;
                block.timestamp = timestamp;
                block.nonce = nonce;
                chain.push_back(block);
            }
            return true;
        }
    };

} // namespace JsonBlockchain