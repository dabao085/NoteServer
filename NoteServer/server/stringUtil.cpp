#include "stringUtil.h"

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