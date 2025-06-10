# include "stringutils.h"
#include "../Library/library.h"

/*update.cpp*/
namespace StringUtils{
    void update_Function_main(std::string &s, int pos)
    {
        bool inSingleQuote = false;
        bool inDoubleQuote = false;
        
        char lastChar = ';';
        char lastNonWhitespaceChar = ';';
        
        bool foundMainOrBlock = false;
        
        std::string prefix;
        std::string targetCode = s;

        if (pos >= 0) {
            prefix = targetCode.substr(0, pos);
            targetCode = targetCode.substr(pos);
        }

        for (int i = 0; i < targetCode.size() && !foundMainOrBlock;) {
            if (targetCode[i] == '\\') {
                i += 2;
                continue;
            }

            if (!inDoubleQuote && targetCode[i] == '\'') {
                inSingleQuote = !inSingleQuote;
                i++;
                continue;
            }

            if (!inSingleQuote && targetCode[i] == '"') {
                inDoubleQuote = !inDoubleQuote;
                i++;
                continue;
            }

            if (inSingleQuote || inDoubleQuote) {
                i++;
                continue;
            }

            if (targetCode[i] == '#') {
                lastChar = lastNonWhitespaceChar = ';';
                while (i < targetCode.size() && targetCode[i] != '\n') i++;
                continue;
            }

            if (lastChar == ';' || lastChar == '}' || std::isspace(static_cast<unsigned char>(lastChar))) {
                if ((lastNonWhitespaceChar == ';' || lastNonWhitespaceChar == '}') && 
                    targetCode[i] == '{') {
                    foundMainOrBlock = true;
                    break;
                }

                if (i + 4 < targetCode.size() && targetCode.substr(i, 4) == "main") {
                    int j = i + 4;
                    while (j < targetCode.size() && std::isspace(static_cast<unsigned char>(targetCode[j]))) j++;
                    if (j < targetCode.size() && targetCode[j] == '(') {
                        foundMainOrBlock = true;
                        break;
                    }
                }
            }

            if (!std::isspace(static_cast<unsigned char>(targetCode[i]))) {
                lastNonWhitespaceChar = targetCode[i];
            }

            lastChar = targetCode[i];
            i++;
        }

        if (!foundMainOrBlock) {
            targetCode = "{" + targetCode + "}\n";
        }

        s = prefix + targetCode;
    }
    std::string update_macro_all(std::string &str) {
        while (1) {
            int i = StringUtils::strpos_exlit_token(str, "all(");
            if (i == -1) break;
            std::string pre = str.substr(0, i);
            std::string sub = str.substr(i);
            int j = StringUtils::find_pairBracket1(sub, 3);
            if (j == -1) break;
            sub = sub.substr(4, j - 4);
            sub = "(" + sub + ").begin(), (" + sub + ").end()";
            std::string tail = str.substr(i + j + 1);
            str = pre + sub + tail;
        }
        return str;
    }

    std::string update_macro_len(std::string &str) {
        while (1) {
            int i = StringUtils::strpos_exlit_token(str, "len(");
            if (i == -1) break;
            std::string pre = str.substr(0, i);
            std::string sub = str.substr(i);
            int j = StringUtils::find_pairBracket1(sub, 3);
            if (j == -1) break;
            sub = sub.substr(4, j - 4);
            sub = "(ll)(" + sub + ").size()";
            std::string tail = str.substr(i + j + 1);
            str = pre + sub + tail;
        }
        return str;
    }

    std::string update_macro_count(std::string &str){
        while(1){
            int i = StringUtils::strpos_exlit_token(str, "Count(");
            if(i == -1) break;
            std::string pre = str.substr(0, i);
            std::string sub = str.substr(i);
            int j = StringUtils::find_pairBracket1(sub, 5);
            if(j == -1) break;
            sub = sub.substr(6, j - 6);
            std::vector<std::string> vars = StringUtils::split_without_char(sub, ',');
            for(auto &x : vars) trim(x);
            sub = "count(" + vars[0] + ".begin(), " + vars[0] + ".end(), " + vars[1] + ")";
            std::string tail = str.substr(i + j + 1);
            str = pre + sub + tail;
        }
        return str;
    }

    std::string updaet_macro_st_bound(std::string &str) {
        while(1){
            int i = StringUtils::strpos_exlit_token(str, "_upper_bound(");
            if(i == -1) break;
            std::string pre = str.substr(0, i);
            std::string sub = str.substr(i);
            int j = StringUtils::find_pairBracket1(sub, 12);
            if(j == -1) break;
            sub = sub.substr(13, j - 13);
            std::vector<std::string> vars = StringUtils::split_without_char(sub, ',');
            for(auto &x : vars) trim(x);
            if(vars.size() == 1) sub = "upper_bound(" + sub + ")";
            else if(vars.size() == 2) sub = vars[0] + ".upper_bound(" + vars[1] + ")";
            else if(vars.size() == 3) {
                std::string tmp = StringUtils::split_without_char(vars[0], '.')[0];
                sub = tmp + ".upper_bound(" + vars[2] + ")";
            }
            std::string tail = str.substr(i + j + 1);
            str = pre + sub + tail;
        }

        while(1){
            int i = StringUtils::strpos_exlit_token(str, "_lower_bound(");
            if(i == -1) break;
            std::string pre = str.substr(0, i);
            std::string sub = str.substr(i);
            int j = StringUtils::find_pairBracket1(sub, 12);
            if(j == -1) break;
            sub = sub.substr(13, j - 13);
            std::vector<std::string> vars = StringUtils::split_without_char(sub, ',');
            for(auto &x : vars) trim(x);
            if(vars.size() == 1) sub = "lower_bound(" + sub + ")";
            else if(vars.size() == 2) sub = vars[0] + ".lower_bound(" + vars[1] + ")";
            else if(vars.size() == 3) {
                std::string tmp = StringUtils::split_without_char(vars[0], '.')[0];
                sub = tmp + ".lower_bound(" + vars[2] + ")";
            }
            std::string tail = str.substr(i + j + 1);
            str = pre + sub + tail;
        }
        return str;
    }
    
    void update_library_replace(std::string &code)
    {
        trim(code);
        replace_AtoZ_exlit_token(code, "ll", "long long");
        replace_AtoZ_exlit_token(code, "ull", "unsigned long long");
        replace_AtoZ_exlit_token(code, "infi", "1073709056");
        replace_AtoZ_exlit_token(code, "INFI", "1073709056");
        replace_AtoZ_exlit_token(code, "infl", "4611686016279904256LL");
        replace_AtoZ_exlit_token(code, "INFL", "4611686016279904256LL");
        replace_AtoZ_exlit_token(code, "inff", "1.2e153");
        replace_AtoZ_exlit_token(code, "vi", "vector<int>");
        replace_AtoZ_exlit_token(code, "VI", "vector<int>");
        replace_AtoZ_exlit_token(code, "vl", "vector<long long>");
        replace_AtoZ_exlit_token(code, "VL", "vector<long long>");
        replace_AtoZ_exlit_token(code, "vll", "vector<long long>");
        replace_AtoZ_exlit_token(code, "VLL", "vector<long long>");
        replace_AtoZ_exlit_token(code, "vs", "vector<string>");
        replace_AtoZ_exlit_token(code, "VS", "vector<string>");

        replace_AtoZ_exlit_token(code, "vvi", "vector<vector<int>>");
        replace_AtoZ_exlit_token(code, "VVI", "vector<vector<int>>");
        replace_AtoZ_exlit_token(code, "vvl", "vector<vector<long long>>");
        replace_AtoZ_exlit_token(code, "VVL", "vector<vector<long long>>");
        replace_AtoZ_exlit_token(code, "vvll", "vector<vector<long long>>");
        replace_AtoZ_exlit_token(code, "VVLL", "vector<vector<long long>>");

        replace_AtoZ_exlit_token(code, "ld", "long double");
        replace_AtoZ_exlit_token(code, "LD", "long double");

        //vector<vector<vector>>
        replace_AtoZ_exlit_token(code, "vvvi", "vector<vector<vector<int>>>");
        replace_AtoZ_exlit_token(code, "VVVI", "vector<vector<vector<int>>>");
        replace_AtoZ_exlit_token(code, "vvvl", "vector<vector<vector<long long>>");
        replace_AtoZ_exlit_token(code, "VVVL", "vector<vector<vector<long long>>");
        replace_AtoZ_exlit_token(code, "vvvll", "vector<vector<vector<long long>>");
        replace_AtoZ_exlit_token(code, "VVVLL", "vector<vector<vector<long long>>");

        //pair
        replace_AtoZ_exlit_token(code, "PII", "pair<int, int>");
        replace_AtoZ_exlit_token(code, "Pll", "pair<long long, long long>");
        replace_AtoZ_exlit_token(code, "PLL", "pair<long long, long long>");

        replace_AtoZ_exlit_token(code, "caesar", "ci.Caesar");
        replace_AtoZ_exlit_token(code, "gcd", "GCD_L");


        //メソッドの呼び出しとか
        // replace_AtoZ_exlit_token(code, "pb", "push_back");
        // replace_AtoZ_exlit_token(code, "eb", "emplace_back");
        // replace_AtoZ_exlit_token(code, "mp", "make_pair");
        // replace_AtoZ_exlit_token(code, "fi", "first");
        // replace_AtoZ_exlit_token(code, "se", "second");
        LibraryManager &libMan = LibraryManager::getInstance();
        if(strpos_exlit_token(code, "UnionFind") >= 0) libMan.requestLibrary("UnionFind", 0);
        if(strpos_exlit_token(code, "WeightedUnionFind") >= 0) libMan.requestLibrary("WeightedUnionFind", 0);
        if(strpos_exlit_token(code, "_SegTree") >= 0) libMan.requestLibrary("_SegTree", 0);
        if(strpos_exlit_token(code, "SegTree_pt_max") >= 0) libMan.requestLibrary("SegTree_pt_max", 0);
        if(strpos_exlit_token(code, "SegTree_pt_min") >= 0) libMan.requestLibrary("SegTree_pt_min", 0);
        if(strpos_exlit_token(code, "SegTree_pt_sum") >= 0) libMan.requestLibrary("SegTree_pt_sum", 0);
        if(strpos_exlit_token(code, "Fenwick_tree") >= 0) libMan.requestLibrary("Fenwick_tree", 0);
        if(strpos_exlit_token(code, "mint") >= 0) libMan.requestLibrary("mint", 0);
        if(strpos_exlit_token(code, "modint") >= 0) libMan.requestLibrary("modint", 0);
        if(strpos_exlit_token(code, "mod_int") >= 0) libMan.requestLibrary("mod_int", 0);
        if(strpos_exlit_token(code, "CycleLib") >= 0) libMan.requestLibrary("CycleLib", 0);
        if(strpos_exlit_token(code, "MexSet") >= 0) libMan.requestLibrary("MexSet", 0);
        // if(strpos_exlit_token(code, "Cipher") >= 0) libMan.insertStill.insert("cipher");
        
        // if(strpos_exlit_token(code, "biseek") >= 0) libMan.insertStill.insert("biseek");
    }
}