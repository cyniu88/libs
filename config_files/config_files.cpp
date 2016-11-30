#include "config_files.h"
#include <cstdio>
#include <cstdlib>

#include <QTextStream>
#include <QDebug>
#include <QThread>
config_Files::config_Files()
{

}

void config_Files::writeToFile(std::string dir, std::string path, std::string value)
{
    QDir myDir;
    qDebug() << " tworze katalog: " << myDir.mkdir(QString::fromStdString(dir));
    myDir.cd(QString::fromStdString(dir));
    QFile file (myDir.absoluteFilePath(QString::fromStdString(path)) );

    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    if (file.isOpen()){
        qDebug() << "plik jest: " << file.isOpen();
    }

    QTextStream stream(&file);
    stream << QString::fromStdString(value);

    file.close();

}

std::string config_Files::readFromFile(std::string dir, std::string path, std::string def  )
{
    QString dirQ = QString::fromStdString(dir);
    QString pathQ = QString::fromStdString(path);
    QString defQ = QString::fromStdString(def);

    QDir myDir(dirQ);

    QFile file (myDir.absoluteFilePath(pathQ));

    // możemy tylko czytać dane, oraz wczytujemy je jako tekst:
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){

        writeToFile(dir, path,def);
        return def;

    }
    // czyścimy wcześniej zapełnioną zmienną tekstową
    QString text;

    QTextStream stream(&file);

    // czytamy wszystkie dane
    text = stream.readAll();

    file.close();

    return text.toStdString();
}
