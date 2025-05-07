#include <regex>
#include <string>
#include <vector>
#include <iostream>

struct Match {
    std::string typeName;
    std::string varName;  // 空文字の可能性あり
};

// pos が "  ' のリテラル内にあるかを判定
bool isInLiteral(const std::string& s, std::size_t pos) {
    bool in_double = false, in_single = false;
    for (std::size_t i = 0; i < pos; ++i) {
        if (s[i] == '\\') {
            // エスケープ文字は次をスキップ
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

std::vector<Match> extractTypeAtVar(const std::string& cond) {
    // 型名：英字＋英数字/_
    // @ の後の変数名は 0文字以上
    std::regex re(R"(([A-Za-z_][A-Za-z0-9_]*)@([A-Za-z_][A-Za-z0-9_]*)?)");
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

int main(){
    std::string cond = R"(while(ptr@node && std::string s = "Type@Fake"; count@))";
    auto matches = extractTypeAtVar(cond);
    for(auto& m : matches){
        std::cout << "型名: " << m.typeName
                  << ", 変数名: " << (m.varName.empty()? "(なし)" : m.varName)
                  << "\n";
    }
    return 0;
}
