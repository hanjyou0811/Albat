#include "lib.h"

#include "Albat/albat.h"
#include "Utils/stringutils.h"

std::string MAIN_BLOCK = "int main()";
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

//とりあえずコード全体をなんてブロック名で囲むかだけ取得する
std::string ReadCmdArg(int argc, char *argv[]) {
    optind = 1; 
    enum {
        EXPORT_OPTION = 1,
    };
    int opt;
    std::string export_value; 
    std::string ret = "";
    struct option long_options[] = {
        {"export", required_argument, 0, EXPORT_OPTION},
        {0, 0, 0, 0}
    };
    while ((opt = getopt_long(argc, argv, "", long_options, NULL)) != -1) {
        switch (opt) {
            case EXPORT_OPTION:
                export_value = optarg;
                StringUtils::trim(export_value);
                if (export_value.size() >= 2) {
                    export_value = export_value.substr(1, export_value.size() - 2);
                    StringUtils::trim(export_value);
                    auto vs = StringUtils::split_without_char(export_value, ',');
                    if (vs.size() < 2) return "";
                    for(int i=0;i<vs.size();i++) {
                        StringUtils::trim(vs[i]);
                        ret += vs[i].substr(1, vs[i].size() - 2);
                        if(i == 0) ret += " ";
                        if(i == 1) ret += "(";
                    }
                    ret += ")";
                } else {
                    return "";
                }
                break;
            case '?':
                return "";
            default:
                break;
        }
    }
    return ret;
}

int main(int argc, char **argv)
{
#ifndef __EMSCRIPTEN__
    if (argc >= 2 && std::string(argv[1]) == "exec") {
        if (argc != 5) {
            fprintf(stderr, "Usage: %s exec <source_file> <input_file> <output_file>\n", argv[0]);
            return 1;
        }

        auto get_exec_dir = []() -> std::string {
            char buffer[PATH_MAX];
            ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
            if (len != -1) {
                buffer[len] = '\0';
                std::string path(buffer);
                size_t pos = path.find_last_of('/');
                if (pos != std::string::npos) {
                    return path.substr(0, pos + 1); 
                }
            }
            return "./";
        };

        std::string cmd = 
            get_exec_dir() + "../run_albat.sh " +
            std::string(argv[2]) + " " + 
            std::string(argv[3]) + " " +
            std::string(argv[4]);
        
        return system(cmd.c_str());
    }
    std::string code, str;
    char buf[10000];
    {
        std::string export_arg = ReadCmdArg(argc, argv);
        if(!export_arg.empty()) {
            MAIN_BLOCK = export_arg;
        }
    }
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
