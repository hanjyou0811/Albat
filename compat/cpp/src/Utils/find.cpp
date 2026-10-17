#include "stringutils.h"

/*find.cpp*/
namespace StringUtils{
    std::vector<std::string> find_Function(std::string &s, std::string format){
        int i, j, k, m, ok;
        int x, y;
        std::vector<std::string> fmt;
        std::vector<std::string> res;

        while (1) {
            std::string tmp;
            trim(format);
            if (format.size() == 0)
            break;

            if (format.substr(0, 2) == "()" || format.substr(0, 2) == "[]" ||
                format.substr(0, 2) == "<>") {
            fmt.push_back(format.substr(0, 2));
            format = format.substr(2);
            continue;
            }

            tmp = "";
            while (format.size() && (isalnum(format[0]) || format[0] == '_')) {
            tmp += format[0];
            format = format.substr(1);
            }
            if (tmp == "") {
            tmp += format[0];
            format = format.substr(1);
            }
            fmt.push_back(tmp);
        }

        x = y = 0;
        for (i = 0; i < s.size(); i++) {
            if (x == 0 && s[i] == '"') {
            y ^= 1;
            continue;
            }
            if (y == 0 && s[i] == '\'') {
            x ^= 1;
            continue;
            }
            if (x || y) {
            if (s[i] == '\\')
                i++;
            continue;
            }

            if (i && (isalnum(fmt[0][0]) || fmt[0][0] == '_') &&
                (isalnum(s[i - 1]) || s[i - 1] == '_'))
            continue;

            res.clear();
            res.push_back(s.substr(0, i));
            j = i;
            ok = 1;
            for (k = 0; k < fmt.size(); k++) {
            while (j < s.size() && isspace(s[j]))
                j++;
            if (j >= s.size()) {
                ok = 0;
                break;
            }

            if (fmt[k] == "()") {
                if (s[j] != '(') {
                ok = 0;
                break;
                }
                m = find_pairBracket1(s, j);
                res.push_back(s.substr(j + 1, m - j - 1));
                j = m + 1;
                while (j < s.size() && isspace(s[j]))
                j++;
                continue;
            }
            if (fmt[k] == "[]") {
                if (s[j] != '[') {
                ok = 0;
                break;
                }
                m = find_pairBracket1(s, j);
                res.push_back(s.substr(j + 1, m - j - 1));
                j = m + 1;
                while (j < s.size() && isspace(s[j]))
                j++;
                continue;
            }
            if (fmt[k] == "<>") {
                if (s[j] != '<') {
                ok = 0;
                break;
                }
                m = find_pairBracket2(s, j);
                res.push_back(s.substr(j + 1, m - j - 1));
                j = m + 1;
                while (j < s.size() && isspace(s[j]))
                j++;
                continue;
            }

            if (s.substr(j, fmt[k].size()) == fmt[k]) {
                j += fmt[k].size();
            } else {
                ok = 0;
                break;
            }
            }
            if (ok) {
            res.push_back(s.substr(j));
            return res;
            }
        }

        res.clear();
        return res;
    }
    std::vector<std::string> find_Function_main(std::string &s, std::string format){
        int i, j, k, m, ok;

        int x, y;

        std::vector<std::string> fmt;
        std::vector<std::string> res;

        while (1) {
            std::string tmp;
            trim(format);
            if (format.size() == 0)
            break;

            if (format.substr(0, 2) == "()" || format.substr(0, 2) == "{}") {
                fmt.push_back(format.substr(0, 2));
                format = format.substr(2);
                continue;
            }

            tmp = "";
            while (format.size() && (isalnum(format[0]) || format[0] == '_')) {
                tmp += format[0];
                format = format.substr(1);
            }
            if (tmp == "") {
                tmp += format[0];
                format = format.substr(1);
            }
            fmt.push_back(tmp);
        }

        x = y = 0;
        for (i = 0; i < s.size(); i++) {
            if (x == 0 && s[i] == '"') {
                y ^= 1;
                continue;
            }
            if (y == 0 && s[i] == '\'') {
                x ^= 1;
                continue;
            }
            if (x || y) {
                if (s[i] == '\\')
                    i++;
                continue;
            }

            if (i && (isalnum(fmt[0][0]) || fmt[0][0] == '_') &&
                (isalnum(s[i - 1]) || s[i - 1] == '_'))
            continue;

            res.clear();
            res.push_back(s.substr(0, i));
            j = i;
            ok = 1;
            for (k = 0; k < fmt.size(); k++) {
                while (j < s.size() && isspace(s[j]))
                    j++;
                if (j >= s.size()) {
                    ok = 0;
                    break;
                }

                if (fmt[k] == "()") {
                    if (s[j] != '(') {
                        ok = 0;
                        break;
                    }
                    m = find_pairBracket1(s, j);
                    res.push_back(s.substr(j + 1, m - j - 1));
                    j = m + 1;
                    while (j < s.size() && isspace(s[j]))
                        j++;
                    continue;
                }
                if (fmt[k] == "{}") {
                    if (s[j] != '{') {
                        ok = 0;
                        break;
                    }
                    m = find_pairBracket1(s, j);
                    res.push_back(s.substr(j + 1, m - j - 1));
                    j = m + 1;
                    while (j < s.size() && isspace(s[j]))
                        j++;
                    continue;
                }

                if (s.substr(j, fmt[k].size()) == fmt[k]) {
                    j += fmt[k].size();
                } else {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                res.push_back(s.substr(j));
                return res;
            }
        }

        res.clear();
        return res;
    }
    int find_Function_main_bool(std::string &s, std::string format)
    {
        int i, j, k, m, ok;
        int x, y;

        std::vector<std::string> fmt;
        std::vector<std::string> res;

        while (1) {
            std::string tmp;
            trim(format);
            if (format.size() == 0)
            break;

            if (format.substr(0, 2) == "()" || format.substr(0, 2) == "{}") {
                fmt.push_back(format.substr(0, 2));
                format = format.substr(2);
                continue;
            }

            tmp = "";
            while (format.size() && (isalnum(format[0]) || format[0] == '_')) {
                tmp += format[0];
                format = format.substr(1);
            }
            if (tmp == "") {
                tmp += format[0];
                format = format.substr(1);
            }
            fmt.push_back(tmp);
        }

        x = y = 0;
        for (i = 0; i < s.size(); i++) {
            if (x == 0 && s[i] == '"') {
                y ^= 1;
                continue;
            }
            if (y == 0 && s[i] == '\'') {
                x ^= 1;
                continue;
            }
            if (x || y) {
            if (s[i] == '\\')
                i++;
            continue;
            }

            if (i && (isalnum(fmt[0][0]) || fmt[0][0] == '_') &&
                (isalnum(s[i - 1]) || s[i - 1] == '_'))
            continue;

            res.clear();
            res.push_back(s.substr(0, i));
            j = i;
            ok = 1;
            for (k = 0; k < fmt.size(); k++) {
                while (j < s.size() && isspace(s[j]))
                    j++;
                if (j >= s.size()) {
                    ok = 0;
                    break;
                }

                if (fmt[k] == "()") {
                    if (s[j] != '(') {
                    ok = 0;
                    break;
                    }
                    m = find_pairBracket1(s, j);
                    res.push_back(s.substr(j + 1, m - j - 1));
                    j = m + 1;
                    while (j < s.size() && isspace(s[j]))
                        j++;
                    continue;
                }
                if (fmt[k] == "{}") {
                    if (s[j] != '{') {
                        ok = 0;
                        break;
                    }
                    m = find_pairBracket1(s, j);
                    res.push_back(s.substr(j + 1, m - j - 1));
                    j = m + 1;
                    while (j < s.size() && isspace(s[j]))
                        j++;
                    continue;
                }

                if (s.substr(j, fmt[k].size()) == fmt[k]) {
                    j += fmt[k].size();
                } else {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                res.push_back(s.substr(j));
                return res.size() > 0;
            }
        }

        res.clear();
        return res.size() > 0;
    }
    int find_pairBracket1(std::string &s, int pos){
        int in_Parentheses, in_Brackets, in_Braces, in_String, in_Char;

        in_Parentheses = in_Brackets = in_Braces = in_String = in_Char = 0;
        if (s[pos] == '(' || s[pos] == '[' || s[pos] == '{') {
            for (int i = pos; i < s.size(); i++) {
            if (in_String == 0 && in_Char == 0) {
                if (s[i] == '(')
                in_Parentheses++;
                if (s[i] == ')')
                in_Parentheses--;
                if (s[i] == '[')
                in_Brackets++;
                if (s[i] == ']')
                in_Brackets--;
                if (s[i] == '{')
                in_Braces++;
                if (s[i] == '}')
                in_Braces--;
            }
            if (in_String == 0 && s[i] == '"')
                in_Char ^= 1;
            if (in_Char == 0 && s[i] == '\'')
                in_String ^= 1;
            if ((in_String || in_Char) && s[i] == '\\') {
                i++;
                continue;
            }
            if (in_Parentheses == 0 && in_Brackets == 0 && in_Braces == 0 && in_String == 0 && in_Char == 0)
                return i;
            }
        } else if (s[pos] == ')' || s[pos] == ']' || s[pos] == '}') {
            for (int i = pos; i >= 0; i--) {
            if (in_String == 0 && in_Char == 0) {
                if (s[i] == ')')
                in_Parentheses++;
                if (s[i] == '(')
                in_Parentheses--;
                if (s[i] == ']')
                in_Brackets++;
                if (s[i] == '[')
                in_Brackets--;
                if (s[i] == '}')
                in_Braces++;
                if (s[i] == '{')
                in_Braces--;
            }
            if (in_String == 0 && s[i] == '"')
                in_Char ^= 1;
            if (in_Char == 0 && s[i] == '\'')
                in_String ^= 1;
            if ((in_String || in_Char) && s[i] == '\\') {
                i--;
                continue;
            }
            if (in_Parentheses == 0 && in_Brackets == 0 && in_Braces == 0 && in_String == 0 && in_Char == 0)
                return i;
            }
        }
        return -1;
    }
    int find_pairBracket2(std::string &s, int pos){
        int in_Parentheses, in_Brackets, in_Braces, in_String, in_Char;
        std::string st;

        in_Parentheses = in_Brackets = in_Braces = in_String = in_Char = 0;
        if (s[pos] == '(' || s[pos] == '[' || s[pos] == '{' || s[pos] == '<') {
            for (int i = pos; i < s.size(); i++) {
            if (in_String == 0 && in_Char == 0) {
                if (s[i] == '(')
                in_Parentheses++;
                if (s[i] == ')')
                in_Parentheses--;
                if (s[i] == '[')
                in_Brackets++;
                if (s[i] == ']')
                in_Brackets--;
                if (s[i] == '{')
                in_Braces++;
                if (s[i] == '}')
                in_Braces--;
                if (s[i] == '<')
                in_String++;
                if (s[i] == '>')
                in_String--;
                if (st.size() >= 2 && st.substr(st.size() - 2) == "()")
                st = st.substr(0, st.size() - 2);
                if (st.size() >= 2 && st.substr(st.size() - 2) == "[]")
                st = st.substr(0, st.size() - 2);
                if (st.size() >= 2 && st.substr(st.size() - 2) == "{}")
                st = st.substr(0, st.size() - 2);
                if (st.size() >= 2 && st.substr(st.size() - 2) == "<>")
                st = st.substr(0, st.size() - 2);
            }
            if (in_String == 0 && s[i] == '"')
                in_Char ^= 1;
            if (in_Char == 0 && s[i] == '\'')
                in_String ^= 1;
            if ((in_String || in_Char) && s[i] == '\\') {
                i++;
                continue;
            }
            if (st.size() == 0)
                return i;
            }
        } else if (s[pos] == ')' || s[pos] == ']' || s[pos] == '}' || s[pos] == '>') {
            for (int i = pos; i >= 0; i--) {
            if (in_String == 0 && in_Char == 0) {
                if (s[pos] == '(')
                st += s[pos];
                if (s[pos] == ')')
                st += s[pos];
                if (s[pos] == '[')
                st += s[pos];
                if (s[pos] == ']')
                st += s[pos];
                if (s[pos] == '{')
                st += s[pos];
                if (s[pos] == '}')
                st += s[pos];
                if (s[pos] == '<')
                st += s[pos];
                if (s[pos] == '>')
                st += s[pos];
                if (st.size() >= 2 && st.substr(st.size() - 2) == ")(")
                st = st.substr(0, st.size() - 2);
                if (st.size() >= 2 && st.substr(st.size() - 2) == "][")
                st = st.substr(0, st.size() - 2);
                if (st.size() >= 2 && st.substr(st.size() - 2) == "}{")
                st = st.substr(0, st.size() - 2);
                if (st.size() >= 2 && st.substr(st.size() - 2) == "><")
                st = st.substr(0, st.size() - 2);
            }
            if (in_String == 0 && s[i] == '"')
                in_Char ^= 1;
            if (in_Char == 0 && s[i] == '\'')
                in_String ^= 1;
            if (st.size() == 0)
                return i;
            }
        }
        return -1;
    }
}