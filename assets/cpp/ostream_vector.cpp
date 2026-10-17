template <class T>
ostream &operator<<(ostream &os, const vector<T> &v) {
    fore(x, v) os << x << ' ';
    return os;
}
