template<typename T>
vector<T> bit_enumerate(T x) {
    vector<T> res;
    for (T s = x; ; s = (s - 1) & x) {
        res.push_back(s);
        if (s == 0) break;
    }
    return res;
}
