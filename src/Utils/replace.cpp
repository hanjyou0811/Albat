# include "stringutils.h"
#include <iostream>

/*replace.cpp*/
namespace StringUtils {
    int replace_AtoZ(std::string &str, std::string A, std::string Z) {
        int i, res = 0;
        while (1) {
            i = strpos(str, A);
            if (i == -1) break;
            str = str.substr(0, i) + Z + str.substr(i + A.size());
            res++;    
        }
        return res;
    }

    int replace_AtoZ_exlit_token(std::string &str, std::string A, std::string Z)
    {
        int i, res = 0;
        while(1)
        {
            i = strpos_exlit_token(str, A);
            if(i == -1) break;
            str = str.substr(0, i) + Z + str.substr(i + A.size());
            res++;
        }
        return res;
    }

    int replace_AtoZ_exlit_token_with_digit(std::string &S, std::string &T, std::string &C) {
        int res, x, y;
        int size_S, size_T;
        size_S = S.size();
        size_T = T.size();
        res = 0;
        x = y = 0;
        for (int i = 0; i < size_S - size_T + 1; i++) {
            if ((x || y) && S[i] == '\\') {
                i += 2;
                continue;
            }
            if (x == 0 && S[i] == '"') {
                y ^= 1;
                i++;
                continue;
            }
            if (y == 0 && S[i] == '\'') {
                x ^= 1;
                i++;
                continue;
            }
            if (x == 0 && y == 0 && S.compare(i, size_T, T) == 0) {
                if (i + size_T < size_S &&
                    (isalnum(S[i + size_T]) || S[i + size_T] == '_')) {
                    i += size_T;
                    continue;
                }
                int j = 0;
                int digitcount = 0;
                int isDigit = 0;

                while (i - j - 1 >= 0 && (isdigit(S[i - j - 1]) || isspace(S[i - j - 1]))) {
                    if (isdigit(S[i - j - 1])) {
                        digitcount++;
                        isDigit = 1;
                    }
                    j++;
                }
                if (isDigit && (i - j >= 0 && (isalnum(S[i - j]) || S[i - j] == '_'))) {
                    i += size_T;
                    continue;
                }
                if (digitcount) {
                    S = S.substr(0, i) + "*" + C + S.substr(i + size_T);
                    i += C.size() + 1;
                } else {
                    S = S.substr(0, i) + C + S.substr(i + size_T);
                    i += C.size();
                }
                res++;
                size_S = S.size();
            } else {
                i++;
            }
    }
        return res;
    }
}