#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "parser.hpp"


std::map<std::string, std::string> read_config  ( const char* file_path    )
{
    std::string content ;
    std::string variable ;
    std::string v_value ;
    std::fstream config_file;

    std::map<std::string, std::string> configMAP;

    config_file.open(file_path, std::ios::in  );
    if( config_file.good() == false )
    {
        log_file_mutex.mutex_lock();
        log_file_cout << "Brak pliku konfiguracyjnego" << std::endl;
        log_file_mutex.mutex_unlock();
        throw "cannot read config";
    }

    while( !config_file.eof() )
    {
        getline (config_file, content);

        for (unsigned i=0;  i<content.length(); ++i)
        {
            if ( content.at(i)==':' && content.at(i+1)=='=')
            {   for (unsigned int j=i+2;j<content.length(); ++j )
                {
                    if ( content.at(j)==' '|| content.at(j)==0x9 ){   // przerwij odczyt  jesli znajdzesz spacje lub tab
                        break;
                    }
                    // std::cout << (int)content.at(j);
                    v_value.push_back(content.at(j));  // v_value zmiennej
                }
                break;
            }
            if (content.at(i)!=' ' )
            {
                variable.push_back(content.at(i));  // variable
            }
        }
        configMAP.insert(std::make_pair(variable, v_value));
      }
    config_file.close();

    return configMAP;
}
