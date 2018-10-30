#include "stringUtil.h"

/*std::string & std_string_format(std::string & _str, const char * _Format, ...) {
	std::string tmp;
 
	va_list marker = NULL;
	va_start(marker, _Format);
 
	size_t num_of_chars = _vscprintf(_Format, marker);
 
	if (num_of_chars > tmp.capacity()) {
		tmp.resize(num_of_chars + 1);
	}
 
	vsprintf_s((char *)tmp.data(), tmp.capacity(), _Format, marker);
 
	va_end(marker);
 
	_str = tmp.c_str();
	return _str;
}*/
 
/*std::string & std_string_format(std::string & _str, const char * _Format, ...){
	va_list marker = {NULL};
	va_start(marker, _Format);
 
	size_t num_of_chars = _vscprintf(_Format, marker);
 
	if (num_of_chars > _str.capacity()){
		_str.resize(num_of_chars + 1);
	}
 
	vsprintf((char *) _str.c_str(), _Format, marker);
 
	va_end(marker);
	return _str;
}*/

/*inputParam1:request is a string which contains command and parameters etc.
 *inputParam2: queryElements are the result parsed by request, split by inputParam3
 *inputParam3: split character
*/
void splitStr(const std::string &request, const std::string &splitChars, std::vector<std::string> &queryElements){
    char pStr[1024], pSplitChars[16];
    request.copy(pStr, request.length(), 0);
    splitChars.copy(pSplitChars, splitChars.length(), 0);
    char *pch;
    pch = strtok(pStr, pSplitChars);
    while (pch != NULL)
    {
        queryElements.push_back(std::string(pch));
        pch = strtok(NULL, pSplitChars);
    }
}