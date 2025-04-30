#include "../albat.h"

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
        "string","auto","cpp_int", "mint", "modint", "mod_int", "MexSet", "UnionFind", "auto"};

    STLtypenames = {"pair",         "tuple", "vector",         "stack",
            "queue",        "deque", "priority_queue", "set",
            "multiset",     "map",   "bitset",         "unordered_set",
            "unordered_map", "Fenwick_tree","_SegTree", "SegTree_pt_max",
            "SegTree_pt_min", "SegTree_pt_sum", "WeightedUnionFind"
            };
}