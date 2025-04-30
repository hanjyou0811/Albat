#include "stringutils.h"

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
        while((!isalnum(str[i]) && str[i] != '!' && str[i] != '\'' && str[i] != '\"') && i < str.size()) i++;
        while((isalnum(str[i]) || str[i] == '!' || str[i] == '\'' || str[i] == '\"') && i < str.size())
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

}