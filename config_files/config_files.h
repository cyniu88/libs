#ifndef CONFIG_FILES_H
#define CONFIG_FILES_H

#include <QFile>
#include <QDir>

class config_Files
{
public:
    config_Files();

    void writeToFile(QString dir,QString path, QString value);
    QString readFromFile(QString dir, QString path, QString def = "not set");
};

#endif // CONFIG_FILES_H
