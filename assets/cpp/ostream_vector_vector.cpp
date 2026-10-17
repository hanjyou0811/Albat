template <class T>
ostream &operator<<(ostream &os, const vector<vector<T>> &v) {
    fore(vv, v) {
        fore(x, vv) os << x << ' ';
        os << '\n';
    }
    return os;
}
