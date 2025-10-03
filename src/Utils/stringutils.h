#ifndef STRINGUTILS_H
#define STRINGUTILS_H

# include <string>
# include <utility>
# include <vector>
# include <algorithm>
# include <regex>

struct Match {
    std::string typeName;
    std::string varName;
};

namespace StringUtils {
    int isOperator(char c);

    /*trim*/
    //前から
    void ftrim(std::string &s);
    //後ろから
    void btrim(std::string &s);
    //前後
    void trim(std::string &s);
    //空白消す
    void removeAllWhitespace(std::string &s);
    void ftrim_char(std::string &s, char c);
    void btrim_char(std::string &s, char c);
    void trim_char(std::string &s, char c, char t);

    /*strpos*/
    int strpos(std::string &S, std::string &T, int pos = 0);
    int strpos_exlit(std::string &S, std::string T, int pos = 0);
    int strrpos_exlit(std::string &S, std::string T, int pos = -1);
    int strpos_token(std::string &S, std::string T, int pos = 0);
    int strpos_exlit_token(std::string &S, std::string T, int pos = 0);

    int replace_AtoZ(std::string &str, std::string A, std::string Z);
    int replace_AtoZ_exlit_token(std::string &str, std::string A, std::string Z);
    int replace_AtoZ_exlit_token_with_digit(std::string &S, std::string &T, std::string &C);

    std::pair<std::string, char> nextToken(std::string &str);

    std::vector<std::string> split_without_char(std::string s, char c);
    std::vector<std::string> split_without_chars(std::string s, std::string cs);
    std::vector<std::string> split_without_str(std::string s, std::string str);

    std::vector<std::string> find_Function(std::string &s, std::string format);
    std::vector<std::string> find_Function_main(std::string &s, std::string format);
    int find_pairBracket1(std::string &s, int pos);
    int find_pairBracket2(std::string &s, int pos);

    std::string extra_varname(std::string &str);
    std::string extra_varname_exlit(std::string &str);
    std::string extra_varname_for_vardef(std::string &str);
    std::string extra_input_option(std::string &str);
    std::pair<std::string, std::string> extra_VarInfo(const std::string &str);
    std::vector<Match> extractTypeAtVar(const std::string& cond);

    void update_Function_main(std::string &s, int pos = -1);
    void update_library_replace(std::string &code);
    std::string update_macro_all(std::string &str);
    std::string update_macro_len(std::string &str);
    std::string updaet_macro_st_bound(std::string &str);
    std::string update_macro_count(std::string &str);
    // fmtを受け取る
    std::string update_slice(std::vector<std::string> &fmt);

    int size_block(std::string &str);
    int size_brackets(std::string &st);

    std::vector<std::pair<std::string,std::string>> def_or_input(std::string type_str, std::string vardef_str);
    void library_replace(std::string &code);
    
    std::string extra_varname(std::string &str);
    std::string extra_varname_exlit(std::string &str);
    std::string extra_input_option(std::string &str);
} // namespace StringUtils
#endif