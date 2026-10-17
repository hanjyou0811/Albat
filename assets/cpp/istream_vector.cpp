template <class T>
istream &operator>>(istream &is, vector<T> &v) {
    fore(x, v) is >> x;
    return is;
}
