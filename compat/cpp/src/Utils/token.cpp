#include "stringutils.h"

/*token.cpp*/
namespace StringUtils{
    std::pair<std::string, char> nextToken(std::string &str){
        int i, ok = 0;

        std::string res1 = "";
        char res2 = '\0';

        i = 0;
        while(i < str.size() && isspace(str[i]))
        {
            i++;
        }
        if(i == str.size())
        {
            return make_pair(res1,res2);
        }
        if(isdigit(str[0]) || (str[0]=='.' && isdigit(str[1])))
        {
            ok = 1;
        }
        while(i < str.size() && (isalnum(str[i]) || str[i]=='.' || (!ok && str[i]=='_')))
        {
            res1 += str[i];
            i++;
        }

        while(i < str.size() && isspace(str[i])){
            i++;
        }
        res2 = str[i];
        return make_pair(res1,res2);
    }
}