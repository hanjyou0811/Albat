#include "lib.h"

#include "Albat/albat.h"
#include "Utils/stringutils.h"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

const char* processString(const char* input);

namespace {

std::string NormalizeSourcePath(const std::filesystem::path &inputPath)
{
    if (inputPath.empty()) {
        return "<stdin>";
    }
    std::error_code ec;
    std::filesystem::path normalized = inputPath.lexically_normal();
    if (!normalized.is_absolute()) {
        return normalized.string();
    }
    std::filesystem::path current = std::filesystem::current_path(ec);
    if (ec) {
        return normalized.string();
    }
    std::filesystem::path relative = normalized.lexically_relative(current);
    if (!relative.empty()) {
        return relative.lexically_normal().string();
    }
    return normalized.string();
}

int RunProcess(const std::vector<std::string> &args)
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        std::vector<char *> argv;
        argv.reserve(args.size() + 1);
        for (const std::string &arg : args) {
            argv.push_back(const_cast<char *>(arg.c_str()));
        }
        argv.push_back(nullptr);
        execvp(argv[0], argv.data());
        perror(argv[0]);
        _exit(127);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return 1;
    }
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }
    return 1;
}

int CompileAndRunSource(const std::string &sourcePath)
{
    std::ifstream input(sourcePath);
    if (!input) {
        fprintf(stderr, "albat: %s: could not open file\n", sourcePath.c_str());
        return 1;
    }

    std::string code((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    ALBAT_SOURCE_FILE = NormalizeSourcePath(std::filesystem::path(sourcePath));

    std::string cppCode = processString(code.c_str());

    std::filesystem::path tmpDir = std::filesystem::temp_directory_path();
    std::string suffix = std::to_string(getpid());
    std::filesystem::path tmpCpp = tmpDir / ("albat_" + suffix + ".cpp");
    std::filesystem::path tmpBin = tmpDir / ("albat_" + suffix);

    {
        std::ofstream out(tmpCpp);
        if (!out) {
            fprintf(stderr, "albat: failed to write temp file: %s\n", tmpCpp.string().c_str());
            return 1;
        }
        out << cppCode << '\n';
    }

    int compileStatus = RunProcess({"g++", "-O2", "-std=gnu++20", "-o",
                                    tmpBin.string(), tmpCpp.string()});
    std::error_code ec;
    std::filesystem::remove(tmpCpp, ec);
    if (compileStatus != 0) {
        std::filesystem::remove(tmpBin, ec);
        return compileStatus;
    }

    int runStatus = RunProcess({tmpBin.string()});
    std::filesystem::remove(tmpBin, ec);
    return runStatus;
}

} // namespace

std::string MAIN_BLOCK = "int main()";
std::string ALBAT_SOURCE_FILE = "<stdin>";
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
    albat.parse(code, "", 0, LINETYPES::PROGRAM, 1, ALBAT_SOURCE_FILE);

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

std::string DetectSourceFileFromStdin()
{
#ifdef __EMSCRIPTEN__
    return "<stdin>";
#else
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/fd/0", buffer, sizeof(buffer) - 1);
    if (len <= 0) {
        return "<stdin>";
    }
    buffer[len] = '\0';
    std::filesystem::path path(buffer);
    if (path.empty() || !path.is_absolute()) {
        return "<stdin>";
    }
    return NormalizeSourcePath(path);
#endif
}

int main(int argc, char **argv)
{
#ifndef __EMSCRIPTEN__
    if (argc >= 2 && std::string(argv[1]) == "run") {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s run <source_file.al>\n", argv[0]);
            return 1;
        }
        return CompileAndRunSource(argv[2]);
    }

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
    ALBAT_SOURCE_FILE = DetectSourceFileFromStdin();
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
