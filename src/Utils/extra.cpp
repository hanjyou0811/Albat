#include "stringutils.h"

bool isInLiteral(const std::string& s, std::size_t pos) {
    bool in_double = false, in_single = false;
    for (std::size_t i = 0; i < pos; ++i) {
        if (s[i] == '\\') {
            ++i;
            continue;
        }
        if (!in_single && s[i] == '"') {
            in_double = !in_double;
        }
        else if (!in_double && s[i] == '\'') {
            in_single = !in_single;
        }
    }
    return in_double || in_single;
}

/*extra.cpp*/
namespace StringUtils
{
    std::string extra_varname(std::string &str)
    {
        std::string res = "";
        int i = 0;
        while((!isalnum(str[i])  && str[i] != '!') && i < str.size()) i++;
        while((isalnum(str[i]) || str[i] == '!') && i < str.size())
        {
            res += str[i];
            i++;
        }
        return res;
    }

    std::string extra_varname_exlit(std::string &str)
    {
        std::string res = "";
        int i = 0;
        while((!isalnum(str[i]) && str[i] != '\'' && str[i] != '\"') && i < str.size()) i++;
        while((isalnum(str[i]) || str[i] == '\'' || str[i] == '\"' || StringUtils::isOperator(str[i])) && i < str.size())
        {
            res += str[i];
            i++;
        }
        return res;
    }

    std::string extra_varname_for_vardef(std::string &str)
    {
        std::string res = "";
        int i = 0;
        while((!isalnum(str[i]) && str[i] != '\'' && str[i] != '\"' && str[i] != '*' && str[i] != '&') && i < str.size()) i++;
        while((isalnum(str[i]) || isspace(str[i]) || str[i] == '\'' || str[i] == '\"' || str[i] == '*' || str[i] == '&' || str[i] == '=') && i < str.size())
        {
            res += str[i];
            i++;
        }
        return res;
    }

    std::string extra_input_option(std::string &str)
    {
        std::string tmp = str;
        int t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
        int cnt = 0;
        std::string res = "";
        for(int i=str.size()-1;i>=0;i--)
        {
            if(t4 == 0 && t5 == 0)
            {
                if(str[i] == '(') t1++;
                if(str[i] == ')') t1--;
                if(str[i] == '[') t2++;
                if(str[i] == ']') t2--;
                if(str[i] == '{') t3++;
                if(str[i] == '}') t3--;
            }
            if(t4 == 0 && str[i] == '"') t5 ^= 1;
            if(t5 == 0 && str[i] == '\'') t4 ^= 1;
            if(t1 == 0 && t2 == 0 && t3 == 0 && t4 == 0 && t5 == 0)
            {
                res = str.substr(i);
                break;
            }
        }
        return res.substr(1,res.size()-2);
    }
    std::pair<std::string, std::string> extra_VarInfo(const std::string &str)
    {
        std::string name = "", value = "";
        auto vs = StringUtils::split_without_char(str, '=');
        if(vs.size() == 2)
        {
            name = vs[0];
            value = vs[1];
            StringUtils::trim(name);
            StringUtils::trim(value);
        }
        else if(vs.size() == 1)
        {
            name = vs[0];
            StringUtils::trim(name);
        }
        else
        {
            name = str;
            StringUtils::trim(name);
        }
        return std::make_pair(name, value);
    }
    std::vector<Match> extractTypeAtVar(const std::string& cond) {
        // 型名：英字＋英数字/_
        // @ の後の変数名は 0文字以上
        std::regex re(R"(([A-Za-z_][A-Za-z0-9_]*)?@([A-Za-z_][A-Za-z0-9_]*)?)");
        auto begin = std::sregex_iterator(cond.begin(), cond.end(), re);
        auto end   = std::sregex_iterator{};
        std::vector<Match> results;
    
        for (auto it = begin; it != end; ++it) {
            const std::smatch& m = *it;
            std::size_t start = m.position(0);
            // リテラル内ならスキップ
            if (isInLiteral(cond, start)) continue;
            results.push_back({
                m[1].str(),
                m[2].str()
            });
        }
        return results;
    }
}