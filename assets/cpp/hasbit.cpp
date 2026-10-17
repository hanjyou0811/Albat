template<typename T>
bool hasbit(T x, T k) {
    return ((x >> k) & 1) != 0;
}
