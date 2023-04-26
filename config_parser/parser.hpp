#ifndef PARSER_H_
#define PARSER_H_

#include <map>
#include <string>
namespace PARSER {
std::map<std::string, std::string> read_config(const char *file_path);
} // namespace PARSER
#endif
