#include "../albat.h"
#include "../../Utils/stringutils.h"

std::string readFileToString(const fs::path& absolute_path) {
    #ifdef __EMSCRIPTEN__
    return "";
    #endif
    if (!absolute_path.is_absolute()) {
        return "";
    }

    std::ifstream file(absolute_path, std::ios::in);

    if (!file.is_open()) {
        return "";
    }
    try {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    } catch (const std::exception& e) {
        return "";
    }
}

std::string GetWorkingDir() {
    #ifdef __EMSCRIPTEN__
    return "";
    #endif
    try {
        return fs::current_path().string();
    } catch (const fs::filesystem_error& e) {
        // std::cerr << "Error getting current path: " << e.what() << std::endl;
        return "";
    }
}

//lineに対しての処理が適当なので後から直す(返り値をstd::stringにするなど)
void Albat::Preprocess_handle(std::string &line, LINETYPES type)
{
    if(type == LINETYPES::DEFINE)
    {
        auto vs = StringUtils::split_without_char(line, ' ');
        if(vs.size() < 3) {
            return ;
        }
        std::string directive = vs[0];
        std::string define_A = vs[1];
        std::string define_B = line.substr(directive.size() + define_A.size() + 2);
        StringUtils::trim(define_B);
        if (isValidtypename(define_B, ' ')) {
            addTempVarType(define_A);
        }
        line = "#" + line;
        return ;
    }
    if(type == LINETYPES::INCLUDE)
    {
        auto IncludeFilenameValidate = [](const std::string &filename) -> bool {
            return true;
        };
        //#include file でfile開いてくっつける的な処理をかく
        auto vs = StringUtils::split_without_char(line, ' ');
        if(vs.size() != 2) return ;
        std::string filename = vs[1];
        if(!IncludeFilenameValidate(filename)) {
            return ;
        }
        std::string filecontent = readFileToString(GetWorkingDir() + "/" + filename);
        if(filecontent.empty()) {
            line = "#" + line;
            return ;
        }
        filecontent = "//" + line + "\n" + filecontent;
        insert(filecontent, 0);
        line = "//" + line;
        return ;
    }
}

int Albat::isValidEmptyBlock()
{
    // if()
    for(const auto &line : lines)
    {
        if( line != ";"){
            return (0);
        }
    }
    return (1);
}

int Albat::isValidPreprocessor(std::string &code)
{
    if(code.empty() || code[0] != '#') return (0);
    
    struct PreprocessorInfo {
        std::string name;
        LINETYPES type;
    };
    
    std::string preprocess_line = code.substr(1);
    StringUtils::ftrim(preprocess_line);

    std::vector<PreprocessorInfo> preprocessorList = {
        {"define", LINETYPES::DEFINE},
        {"include", LINETYPES::INCLUDE},
        {"ifdef", LINETYPES::IFDEF},
        {"endif", LINETYPES::ENDIF},
        {"undef", LINETYPES::UNDEF},
        {"pragma", LINETYPES::PRAGMA}
    };
    int need_handle = 0;
    for(const auto &pp : preprocessorList)
    {
        if(preprocess_line.substr(0, pp.name.size()) == pp.name)
        {
            int i = pp.name.size();  // まずディレクティブの名前をスキップ
            
            // 改行かファイル終端まで進む
            while(i < preprocess_line.size() && preprocess_line[i] != '\n' && preprocess_line[i] != '\r')
            {
                // include用特殊処理
                if(pp.type == LINETYPES::INCLUDE)
                {
                    need_handle = 1;
                }
                if(pp.type == LINETYPES::DEFINE)
                {
                    need_handle = 1;
                }
                i++;
            }
            std::string tmp = preprocess_line.substr(0, i);
            if(need_handle) {
                Preprocess_handle(tmp, pp.type);
            }else{
                tmp = "#" + tmp;
            }
            code = preprocess_line.substr(i);
            nextindices.push_back(-1);
            lines.push_back(tmp);
            lineTypes.push_back(pp.type);
            
            return (1);
        }
    }
    return (0);
}
int Albat::isValidComment(std::string &code)
{
    int i;
    if(code.substr(0,2) == "//")
    {
        for(i=0;i<code.size();i++)
        {
            if(code[i] == '\n' || code[i] == '\r')
            {
                break;
            }
        }
        code = code.substr(i);
        return (1);
    }
    if(code.substr(0,2) == "/*")
    {
        for(i=1;i<code.size();i++)
        {
            if(code.substr(i-1,2) == "*/")
            {
                i++;
                break;
            }
        }
        code = code.substr(i);
        return (1);
    }
    return (0);
}

//1 : typenames, 2 : STLtypenames, 3 : tmptypenames
int Albat::isValidtypename(std::string &str, char nex)
{
    if(str.empty()) return (0);
    if(str.back() != '>' && isalnum(nex))
    {
        return (0);
    }
    if((isalnum(str.back()) || str.back() == '_') && (isalnum(nex) || nex == '_'))
    {
        return (0);
    }
    const std::vector<std::string> modifiers = {
        "~", "const ", "static ", "typename ", "inline "
    };
    int modified = 1;
    while(modified)
    {
        modified = 0;
        for(const auto &mod : modifiers)
        {
            if(str.substr(0, mod.size()) == mod)
            {
                str = str.substr(mod.size());
                StringUtils::ftrim(str);
                modified = 1;
                break;
            }
        }
    }
    if( typenames.count(str) )
    {
        return (1);
    }
    if( STLtypenames.count(str) )
    {
        return (2);
    }
    if( tmptypenames.count(str) )
    {
        return (3);
    }
    int i = 0;
    while( i < str.size() && (isalnum(str[i]) || str[i] == '_'))
    {
        i++;
    }
    if(i < str.size() && str[i] == '<')
    {
        int j = StringUtils::find_pairBracket2(str, i);
        if(j == -1) return (0);
        std::string suffix = str.substr(j+1);
        StringUtils::trim(suffix);
        if(!suffix.empty())
        {
            if(suffix.substr(0,6) == "::type")
            {
                suffix = suffix.substr(6);
                StringUtils::trim(suffix);
                if(suffix.empty())
                {
                    return (1);
                }
            }
        }
    }
    for(i = 0; i < str.size(); i++)
    {
        if(STLtypenames.count(str.substr(0,i)) || tmptypenames.count(str.substr(0,i)))
        {
            int cnt = 0;
            for(int j = i; j < str.size(); j++)
            {
                if(cnt == 0 && isalnum(str[j])) break;
                if(str[j] == '<') cnt++;
                if(str[j] == '>') cnt--;
                if(cnt == 0 && str[j] == '>')
                {
                    j++;
                    if(j == str.size()) return (1);
                    break;
                }
            }
        }
    }
    return (0);
}

int Albat::isValidFunction(const std::string &code)
{
    std::string tmp;
    for (int i = 0; i < code.size(); i++) {
        if (code[i] == '=') {
            tmp = code.substr(0, i);
            if (StringUtils::strpos_exlit_token(tmp, "operator") == -1) {
                return false;
            }
        }
        
        if (code[i] == ';') return false;
        
        if (code[i] == '(') {
            tmp = code;
            int j = StringUtils::find_pairBracket1(tmp, i);
            j++;
            
            while (j < code.size() && isspace(code[j])) j++;
            
            if (j < code.size() && (code[j] == '{' || code[j] == ':' || code.substr(j, 2) == "->")) {
                return true;
            }
        }
    }
    
    return false;
}