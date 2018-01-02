#ifndef PARSER_H_
#define PARSER_H_
#include <string>
#include <vector>

#include "../c_master_irda/master_irda.h"

std::map<std::string, std::string> read_config(const char* file_path  );
#endif
