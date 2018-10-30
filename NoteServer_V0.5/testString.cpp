#include "stringUtil.h"
#include <iostream>
using namespace std;

int main()
{
    string str;
    char *helloStr[32] = {"cheng"};
    cout << std_string_format(str, "hello world %d", helloStr);

    return 0;
}