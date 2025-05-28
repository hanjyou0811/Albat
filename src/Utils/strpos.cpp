# include "stringutils.h"

/*strpos.cpp*/
namespace StringUtils{
    int strpos(std::string &S, std::string &T, int pos) {
        int size_S, size_T;
        size_S = S.size();
        size_T = T.size();
    
        for (int i = pos; i < size_S - size_T + 1; i++) {
            if (S.substr(i, size_T) == T)
            return i;
        }
        return -1;
    }
    int strpos_exlit(std::string &S, std::string T, int pos) {
        int size_S, size_T;
        int x, y;
        size_S = S.size();
        size_T = T.size();
        x = y = 0;
        for (int i = pos; i < size_S - size_T + 1; i++) {
            if ((x || y) && S[i] == '\\') {
                i++;
                continue;
            }
            if (x == 0 && S[i] == '"')
                y ^= 1;
            if (y == 0 && S[i] == '\'')
                x ^= 1;
            if (x == 0 && y == 0 && S.substr(i, size_T) == T)
            return i;
        }
        return -1;
    }
    int strrpos_exlit(std::string &S, std::string T, int pos) {
            int size_S, size_T;
            int x, y;
    
            size_S = S.size();
            size_T = T.size();
            x = y = 0;
            for (int i = pos; i >= 0; i--) {
                if ((x || y) && S[i] == '\\') {
                i++;
                continue;
                }
                if (x == 0 && S[i] == '"')
                y ^= 1;
                if (y == 0 && S[i] == '\'')
                x ^= 1;
                if (x == 0 && y == 0 && S.substr(i, size_T) == T)
                return i;
            }
            return -1;
    }
    int strpos_token(std::string &S, std::string T, int pos) {
            int size_S, size_T;
            size_S = S.size();
            size_T = T.size();
    
            for (int i = pos; i < size_S - size_T + 1; i++) {
                if (S.substr(i, size_T) == T) {
                    if (i && (isalnum(S[i - 1]) || S[i - 1] == '_'))
                        continue;
                    if (i + size_T < size_S && isalnum(S[i + size_T] || S[i + size_T] == '_'))
                        continue;
                    return i;
                }
            }
            return -1;
    }
    int strpos_exlit_token(std::string &S, std::string T, int pos) {
        int size_S, size_T;
        size_S = S.size();
        size_T = T.size();

        int x, y;
        x = y = 0;
        for (int i = pos; i < size_S - size_T + 1; i++) {
            if ((x || y) && S[i] == '\\') {
                i++;
                continue;
            }
            if (x == 0 && S[i] == '"')
                y ^= 1;
            if (y == 0 && S[i] == '\'')
                x ^= 1;
            if (x == 0 && y == 0 && S.substr(i, size_T) == T) {
                if (i && (isalnum(S[i - 1]) || S[i - 1] == '_'))
                    continue;
                if (i + size_T < size_S && isalnum(S[i + size_T] || S[i + size_T] == '_'))
                    continue;
                return i;
            }
        }
        return -1;
    }
}