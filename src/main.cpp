#include "lib.h"

#include "Albat/albat.h"
#include "Utils/stringutils.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
extern "C" {
#define KEEP EMSCRIPTEN_KEEPALIVE
#else
#define KEEP
#endif
KEEP
const char* processString(const char* input)
{
    static std::string output;

    std::string code = input;
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

    output = albat.print(0);
    return output.c_str();
}

int main()
{
#ifndef __EMSCRIPTEN__
    std::string code, str;
    char buf[10000];
    while (1)
    {
        int siz = fread(buf, 1, 10000, stdin);
        code += std::string(buf, siz);
        if (siz < 10000)
            break;
    }
    str = code;
    const char* result = processString(code.c_str());
    printf("%s\n", result);

    printf("//code\n/*\n");
    printf("%s\n", str.c_str());
    printf("*/\n");
#endif
    return 0;
}



#ifdef __EMSCRIPTEN__
} // extern "C"
#endif
