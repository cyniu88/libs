#ifndef CONFIG_FILES_H
#define CONFIG_FILES_H

#include <QFile>
#include <QDir>

class config_Files
{

public:
    config_Files();

    void writeToFile(std::string dir, std::string path, std::string value);
    std::string readFromFile(std::string dir, std::string path, std::string def = "not set");
};

#endif // CONFIG_FILES_H
