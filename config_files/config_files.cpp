#include "config_files.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>


#include <sys/stat.h>
//#include <fstream>
#include <QTextStream>
#include <QDebug>
#include <QThread>

config_Files::config_Files()
{

}

void config_Files::writeToFile(std::string dir, std::string path, std::string value)
{
    [](std::string d)->int {
#ifdef _WIN32
        return  mkdir(d.c_str());
#else

        return  mkdir(d.c_str(),777);
#endif
    }(dir);

    std::string pathFile =    path;
    std::ofstream myfile ;
    myfile.open(pathFile,std::fstream::in | std::fstream::out | std::fstream::trunc);
    if (myfile.is_open())
    {
        myfile << value;
        myfile.close();
        qDebug() <<"JEST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:( "<< QString::fromStdString(pathFile);
        return;
    }
    else {
        qDebug() <<"nie ma !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:( "<< QString::fromStdString(pathFile);
    }
}

std::string config_Files::readFromFile(std::string dir, std::string path, std::string def  )
{
    std::string line;
    std::string ret;
    std::string pathFile =    path;
    std::ifstream myfile (pathFile);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            ret+= line+"\n";
        }
        myfile.close();
    }

    else
    {
        writeToFile(dir, path,def);
        return def;
    }

    return ret;

}
