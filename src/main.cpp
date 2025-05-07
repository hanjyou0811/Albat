#include <bits/stdc++.h>
using namespace std;

#include "Albat/albat.h"
#include "Utils/stringutils.h"

int main()
{
    string code;
    char buf[10000];
    while(1)
    {
        int siz = fread(buf, 1, 10000, stdin);
        code += string(buf, siz);
        if(siz < 10000) break;
    }
    StringUtils::update_Function_main(code);
    Albat albat;
    albat.parse(code, "", 0, LINETYPES::PROGRAM);
    std::string lib;
    lib = LibraryManager::getInstance().extractInsertLibraries("head");
    albat.insert(lib, 0);
    lib = LibraryManager::getInstance().extractInsertLibraries("header_of_main");
    for(int i=0;i<albat.lines.size();i++)
    {
        if(albat.lines[i].substr(0,10) == "int main()")
        {
            albat.insert(lib, i);
            break;
        }
    }
    albat.print(0);
    // albat.debug(0);
    return 0;
}