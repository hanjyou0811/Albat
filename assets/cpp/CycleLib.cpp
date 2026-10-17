template <class T>
struct CycleLib {
    T n;
    CycleLib(T n) : n(n) {}
    CycleLib() : n(0) {}

    
    T move(T s, T i) {
        return _mod(s + i, n);
    }
    T rmove(T s, T i) {
        return _mod(s - i, n);
    }

    
    T dist(T s, T t) {
        return _mod(t - s, n);
    }
    T rdist(T s, T t) {
        return _mod(s - t, n);
    }

    
    T dist_min(T s, T t) {
        T d = _mod(t - s, n);
        chmin(d, n - d);
        return d;
    }

   
    bool is_crs_m(T s, T m, T t) {
        return dist(s, m) <= dist(s, t);
    }
    bool ris_crs_m(T s, T m, T t) {
        return rdist(s, m) <= rdist(s, t);
    }
};
