#ifndef CONFIG_FILES_H
#define CONFIG_FILES_H

#include <QFile>
#include <QDir>

class config_Files
{

public:
    config_Files();

    void writeToFile(const std::string& dir, const std::string& path, const std::string& value);
    std::string readFromFile(const std::string &dir, const std::string& path, const std::string& def = "not set");
};

#endif // CONFIG_FILES_H
