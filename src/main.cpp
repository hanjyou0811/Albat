#include "lib.h"
using namespace std;

#include "Albat/albat.h"
#include "Utils/stringutils.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
extern "C" {
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
const char* processString(const char* input)
{
    static std::string output;

    string code = input;
    StringUtils::update_Function_main(code);
    Albat albat;
    albat.parse(code, "", 0, LINETYPES::PROGRAM);

    std::string lib;
    lib = LibraryManager::getInstance().extractInsertLibraries("head");
    albat.insert(lib, 0);

    lib = LibraryManager::getInstance().extractInsertLibraries("header_of_main");
    for(int i=0; i<albat.lines.size(); i++)
    {
        if(albat.lines[i].substr(0, 10) == "int main()")
        {
            albat.nextPtrs[albat.nextindices[i]]->insert(lib, 0);
            break;
        }
    }

    code = '\n'+albat.print(0);

    return code.c_str();
}
#endif

int main()
{
#ifndef __EMSCRIPTEN__
    string code, str;
    char buf[10000];
    while (1)
    {
        int siz = fread(buf, 1, 10000, stdin);
        code += string(buf, siz);
        if (siz < 10000)
            break;
    }
    str = code;
    StringUtils::update_Function_main(code);
    Albat albat;
    albat.parse(code, "", 0, LINETYPES::PROGRAM);

    std::string lib;
    lib = LibraryManager::getInstance().extractInsertLibraries("head");
    albat.insert(lib, 0);
    lib = LibraryManager::getInstance().extractInsertLibraries("header_of_main");

    for (int i = 0; i < albat.lines.size(); i++)
    {
        if (albat.lines[i].substr(0, 10) == "int main()")
        {
            albat.nextPtrs[albat.nextindices[i]]->insert(lib, 0);
            break;
        }
    }

    albat.print(0);
    printf("//code\n/*\n");
    printf("%s\n", str.c_str());
    printf("*/\n");
#endif
    return 0;
}

#ifdef __EMSCRIPTEN__
} // extern "C"
#endif
