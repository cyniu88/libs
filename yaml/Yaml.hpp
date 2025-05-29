#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

class SimpleYamlCommandParser {
public:
    struct CommandSet {
        std::vector<std::string> lock;
        std::vector<std::string> unlock;
    };

    using CommandMap = std::map<std::string, CommandSet>;

    explicit SimpleYamlCommandParser(const std::string& filename) {
        loadFromFile(filename);
    }
    explicit SimpleYamlCommandParser() {
        
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Nie można otworzyć pliku: " + filename);

        commandData.clear();
        std::string currentKey;
        std::string currentSubKey;

        std::string line;
        int lineNumber = 0;
        int keyIndent = -1;
        int subKeyIndent = -1;

        while (std::getline(file, line)) {
            ++lineNumber;
            removeInlineComment(line);
            if (line.empty() || isWhitespace(line)) continue;

            int indent = countRawIndentation(line);
            std::string content = trimmed(line);

            if (endsWithColon(content)) {
                std::string keyName = content.substr(0, content.size() - 1);

                if (keyIndent == -1 || indent <= keyIndent) {
                    // Top-level key
                    currentKey = keyName;
                    currentSubKey.clear();
                    keyIndent = indent;
                    subKeyIndent = -1;
                } else if (subKeyIndent == -1 || indent > keyIndent) {
                    // Subkey (lock/unlock)
                    currentSubKey = keyName;
                    subKeyIndent = indent;
                    if (currentKey.empty())
                        throw syntaxError(lineNumber, line, "podklucz bez klucza głównego");

                    if (currentSubKey != "lock" && currentSubKey != "unlock")
                        throw syntaxError(lineNumber, line, "nieznany podklucz: " + currentSubKey);
                } else {
                    throw syntaxError(lineNumber, line, "zbyt głębokie lub nieoczekiwane wcięcie dla podklucza");
                }
            } else if (content[0] == '-') {
                if (currentKey.empty() || currentSubKey.empty())
                    throw syntaxError(lineNumber, line, "element listy bez poprzedzającego klucza/podklucza");

                std::string cmd = content.substr(1);
                trim(cmd);
                if (currentSubKey == "lock")
                    commandData[currentKey].lock.push_back(cmd);
                else if (currentSubKey == "unlock")
                    commandData[currentKey].unlock.push_back(cmd);
            } else {
                throw syntaxError(lineNumber, line, "nieoczekiwany format");
            }
        }
    }

    const CommandMap& getCommands() const {
        return commandData;
    }

    std::vector<std::string> getCommandsFor(const std::string& timeKey, const std::string& type) const {
        auto it = commandData.find(timeKey);
        if (it == commandData.end()) {
            throw std::runtime_error("Nie znaleziono interwału: " + timeKey);
        }

        if (type == "lock") return it->second.lock;
        if (type == "unlock") return it->second.unlock;
        throw std::runtime_error("Niepoprawny typ komendy: " + type);
    }

private:
    CommandMap commandData;

    static void removeInlineComment(std::string& s) {
        bool inQuotes = false;
        for (size_t i = 0; i < s.length(); ++i) {
            if (s[i] == '"') inQuotes = !inQuotes;
            else if (s[i] == '#' && !inQuotes) {
                s = s.substr(0, i);
                break;
            }
        }
    }

    static std::string trimmed(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        return (start == std::string::npos || end == std::string::npos) ? "" : s.substr(start, end - start + 1);
    }

    static void trim(std::string& s) {
        s = trimmed(s);
    }

    static bool isWhitespace(const std::string& s) {
        return s.find_first_not_of(" \t\r\n") == std::string::npos;
    }

    static bool endsWithColon(const std::string& s) {
        return !s.empty() && s.back() == ':';
    }

    static int countRawIndentation(const std::string& s) {
        int count = 0;
        for (char c : s) {
            if (c == ' ') count++;
            else if (c == '\t') count += 4;
            else break;
        }
        return count;
    }

    static std::runtime_error syntaxError(int lineNumber, const std::string& line, const std::string& reason) {
        return std::runtime_error("Błąd składni YAML w linii " + std::to_string(lineNumber) + ": \"" + line + "\" – " + reason);
    }
};
