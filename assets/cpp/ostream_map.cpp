template <class T1, class T2>
ostream &operator<<(ostream &os, map<T1, T2> mp) {
    fore(x, mp) {
        os <<  " | " << x.first << "->" << x.second << " | ";
    }
    return os;
}
