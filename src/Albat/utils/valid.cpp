#include "../albat.h"
#include "../../Utils/stringutils.h"
#include <iostream>

struct PreprocessorInfo{
    std::string name;
    LINETYPES type;
    int need_handle;
};

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
    if(code.empty()) return (0);
    
    struct PreprocessorInfo {
        std::string name;
        LINETYPES type;
        bool need_handle;
    };
    
    std::vector<PreprocessorInfo> preprocessorList = {
        {"#include", LINETYPES::INCLUDE, true},
        {"# include", LINETYPES::INCLUDE, true},
        {"#define", LINETYPES::DEFINE, true},
        {"# define", LINETYPES::DEFINE, true},
        {"#ifdef", LINETYPES::IFDEF, true},
        {"# ifdef", LINETYPES::IFDEF, true},
        {"#endif", LINETYPES::ENDIF, false},
        {"# undef", LINETYPES::UNDEF, true},
        {"#undef", LINETYPES::UNDEF, true},
        {"#pragma", LINETYPES::PRAGMA, true},
        {"# pragma", LINETYPES::PRAGMA, true}
    };
    
    for(const auto &pp : preprocessorList)
    {
        if(code.substr(0, pp.name.size()) == pp.name)
        {
            int i = pp.name.size();  // まずディレクティブの名前をスキップ
            
            // 改行かファイル終端まで進む
            while(i < code.size() && code[i] != '\n' && code[i] != '\r')
            {
                // include用特殊処理
                if(pp.type == LINETYPES::INCLUDE)
                {
                    if((code[i] == '>' || code[i] == '"') && i > 0 && 
                       (code[i-1] != '\\' || (i > 1 && code[i-2] == '\\')))
                    {
                        i++;
                        break;
                    }
                }
                i++;
            }
            
            
            std::string tmp = code.substr(0, i);
            code = code.substr(i);
            
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
        "~", "const ", "static ", "typename "
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
    if( typenames.count(str) || STLtypenames.count(str) || tmptypenames.count(str))
    {
        return (1);
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
        if(STLtypenames.count(str.substr(0,i)))
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