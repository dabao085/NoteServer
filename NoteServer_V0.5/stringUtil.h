#include <string>
#include <string.h>
#include <vector>
#include <stdarg.h> 

//std::string & std_string_format(std::string & _str, const char * _Format, ...);

void splitStr(const std::string &request, const std::string &splitChars, std::vector<std::string> &queryElements);