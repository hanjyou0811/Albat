#include "stringutils.h"

/*trim.cpp*/
namespace StringUtils {
    void ftrim(std::string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(),
                                    [](unsigned char ch) { return !isspace(ch); }));
    }

    void btrim(std::string &s) {
        s.erase(find_if(s.rbegin(), s.rend(),
                        [](unsigned char ch) { return !isspace(ch); })
                    .base(),
                s.end());
    }

    void trim(std::string &s) {
        ftrim(s);
        btrim(s);
    }

    void removeAllWhitespace(std::string &s) {
        s.erase(remove_if(s.begin(), s.end(),
                            [](unsigned char ch) { return isspace(ch); }),
                s.end());
    }

    void ftrim_char(std::string &s, char c) {
        s.erase(s.begin(), find_if(s.begin(), s.end(),
                                    [c](unsigned char ch) { return ch == c; }));
    }

    void btrim_char(std::string &s, char c) {
    s.erase(
        find_if(s.rbegin(), s.rend(), [c](unsigned char ch) { return ch == c; })
            .base(),
        s.end());
    }

    void trim_char(std::string &s, char c, char t) {
        ftrim_char(s, c);
        btrim_char(s, t);
    }
}