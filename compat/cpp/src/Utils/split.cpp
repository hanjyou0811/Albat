# include "stringutils.h"

/*split.cpp*/
namespace StringUtils{
    std::vector<std::string> split_without_char(std::string s, char c){
        int in_Parentheses, in_Brackets, in_Braces, in_String, in_Char;
        in_Parentheses = in_Brackets = in_Braces = in_String = in_Char = 0;

        std::vector<std::string> res;
        std::string tmp;

        for(int i=0;i<s.size();i++)
        {
            if(in_String == 0 && in_Char == 0)
            {
                if(s[i] == '(') in_Parentheses++;
                if(s[i] == ')') in_Parentheses--;
                if(s[i] == '[') in_Brackets++;
                if(s[i] == ']') in_Brackets--;
                if(s[i] == '{') in_Braces++;
                if(s[i] == '}') in_Braces--;
            }
            if(in_String == 0 && s[i] == '"') in_Char ^= 1;
            if(in_Char == 0 && s[i] == '\'') in_String ^= 1;
            if(in_Parentheses == 0 && in_Brackets == 0 && in_Braces == 0 && in_String == 0 && in_Char == 0 && s[i] == c && tmp.size())
            {
                res.push_back(tmp);
                tmp = "";
            }
            else
            {
                tmp += s[i];
            }
        }
        if(tmp.size())
          res.push_back(tmp);
        return res;
    }
std::vector<std::string> split_without_chars(std::string s, std::string cs){
        int in_Parentheses, in_Brackets, in_Braces, in_String, in_Char;
        in_Parentheses = in_Brackets = in_Braces = in_String = in_Char = 0;

        auto is_in = [&](char c) -> bool {
            for(auto x: cs) if(x == c) return true;
            return false;
        };

        std::vector<std::string> res;
        std::string tmp;

        for(int i=0;i<s.size();i++)
        {
            if(in_String == 0 && in_Char == 0)
            {
                if(s[i] == '(') in_Parentheses++;
                if(s[i] == ')') in_Parentheses--;
                if(s[i] == '[') in_Brackets++;
                if(s[i] == ']') in_Brackets--;
                if(s[i] == '{') in_Braces++;
                if(s[i] == '}') in_Braces--;
            }
            if(in_Char == 0 && s[i] == '"') in_String ^= 1;
            if(in_String == 0 && s[i] == '\'') in_Char ^= 1;
            if(in_Parentheses == 0 && in_Brackets == 0 && in_Braces == 0 && in_String == 0 && in_Char == 0 && is_in(s[i]) && tmp.size())
            {
                res.push_back(tmp);
                tmp = "";
            }
            else if(!(in_Parentheses == 0 && in_Brackets == 0 && in_Braces == 0 && in_String == 0 && in_Char == 0 && is_in(s[i])))
            {
                tmp += s[i];
            }
        }
        if(tmp.size()) res.push_back(tmp);
        return res;
    }
    std::vector<std::string> split_without_str(std::string s, std::string str){
        std::vector<std::string> res;
        std::string tmp;

        while (1) {
            int pos = strpos_exlit(s, str, 0);
            if (pos == -1) break;
            tmp = s.substr(0, pos);
            res.push_back(tmp);
            s = s.substr(pos + str.size());
        }
        if (!s.empty()) res.push_back(s);

        return res;
    }
    std::vector<std::string> split_without_char_signature(std::string s, char c){
        int in_Parentheses, in_Brackets, in_Braces, in_String, in_Char, in_Angle;
        in_Parentheses = in_Brackets = in_Braces = in_String = in_Char = in_Angle = 0;

        std::vector<std::string> res;
        std::string tmp;

        for(int i=0;i<s.size();i++)
        {
            if(in_String == 0 && in_Char == 0)
            {
                if(s[i] == '(') in_Parentheses++;
                if(s[i] == ')') in_Parentheses--;
                if(s[i] == '[') in_Brackets++;
                if(s[i] == ']') in_Brackets--;
                if(s[i] == '{') in_Braces++;
                if(s[i] == '}') in_Braces--;
                if(s[i] == '<') in_Angle++;
                if(s[i] == '>') in_Angle--;
            }
            if(in_String == 0 && s[i] == '"') in_Char ^= 1;
            if(in_Char == 0 && s[i] == '\'') in_String ^= 1;
            if(in_Parentheses == 0 && in_Brackets == 0 && in_Braces == 0 && in_Angle == 0 && in_String == 0 && in_Char == 0 && s[i] == c && tmp.size())
            {
                res.push_back(tmp);
                tmp = "";
            }
            else
            {
                tmp += s[i];
            }
        }
        if(tmp.size())
          res.push_back(tmp);
        return res;
    }
}