template <class T>
ostream &operator<<(ostream &os, set<T> st) {
    fore(x, st) {
        os << x << " ";
    }
    return os;
}
