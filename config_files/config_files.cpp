#include "config_files.h"
#include <QTextStream>
#include <QDebug>
#include <QThread>
config_Files::config_Files()
{

}

void config_Files::writeToFile(QString dir, QString path, QString value)
{
    QDir myDir;
    qDebug() << " tworze katalog: " << myDir.mkdir(dir);
    myDir.cd(dir);
    QFile file (myDir.absoluteFilePath(path) );

    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    if (file.isOpen()){
        qDebug() << "plik jest: " << file.isOpen();
    }

    QTextStream stream(&file);
    stream << value;

    file.close();
}

QString config_Files::readFromFile(QString dir,QString path, QString def  )
{
    QDir myDir(dir);

    QFile file (myDir.absoluteFilePath(path));

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

    return text;
}
