#include "../albat.h"
#include <cctype>

Albat::Albat() {
    init();
}
Albat::Albat(std::string name, Albat *parent) : name(name), parent(parent) {
    init();
}

void Albat::init()
{
    typenames = {"void",   "char",     "signed char", "unsigned char", "int", "signed int", "unsigned int",
        "signed", "unsigned", "uint", "long","signed long", "unsigned long", "ulong", "long long",
        "signed long long", "unsigned long long",
        "VI",    "vi",
        "VII",   "vii",
        "VVI",   "vvi",
        "VIII",  "viii",
        "VLL",   "vll",
        "VVLL",  "vvll",
        "VVVLL", "vvvll",
        "VVVI",  "vvvi",
        "VS",    "vs",
        "ll",    "ull",
        "size_t", "int8_t", "int16_t", "int32_t", "int64_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t",
        "__int8_t", "__int16_t", "__int32_t", "__int64_t", "__int128_t", "__uint8_t",
        "__uint16_t","__uint32_t","__uint64_t","__uint128_t","float","double","long double","bool",
        "string","auto","cpp_int", "mint", "modint", "mod_int", "MexSet", "UnionFind", "auto", "istream", "ostream"};

    STLtypenames = {"pair",         "tuple", "vector",         "stack",
            "queue",        "deque", "priority_queue", "set",
            "multiset",     "map",   "bitset",         "unordered_set",
            "unordered_map", "Fenwick_tree","_SegTree", "SegTree_pt_max",
            "SegTree_pt_min", "SegTree_pt_sum", "WeightedUnionFind"
            };
    spacenames = {"std::"};
    currentInputLine = 1;
}

int Albat::countNewlines(const std::string &text) const
{
    int count = 0;
    for (char c : text) {
        if (c == '\n') {
            count++;
        }
    }
    return count;
}

void Albat::consumePrefix(std::string &code, int len)
{
    if (len <= 0) {
        return;
    }
    if (len > static_cast<int>(code.size())) {
        len = static_cast<int>(code.size());
    }
    if (currentInputLine > 0) {
        currentInputLine += countNewlines(code.substr(0, len));
    }
    code = code.substr(len);
}

void Albat::trimLeadingInput(std::string &code)
{
    int pos = 0;
    while (pos < static_cast<int>(code.size()) && isspace(static_cast<unsigned char>(code[pos]))) {
        pos++;
    }
    consumePrefix(code, pos);
}

void Albat::addOutputLine(const std::string &line, LINETYPES type, int nextIndex, int sourceLine)
{
    nextindices.push_back(nextIndex);
    lines.push_back(line);
    lineTypes.push_back(type);
    sourceLines.push_back(sourceLine);
}

void Albat::insertOutputLine(int pos, const std::string &line, LINETYPES type, int nextIndex, int sourceLine)
{
    nextindices.insert(nextindices.begin() + pos, nextIndex);
    lines.insert(lines.begin() + pos, line);
    lineTypes.insert(lineTypes.begin() + pos, type);
    sourceLines.insert(sourceLines.begin() + pos, sourceLine);
}
