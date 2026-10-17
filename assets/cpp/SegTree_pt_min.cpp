template <class S> struct SegTree_pt_min {
  public:
    SegTree_pt_min(int n) : _n(n) {
        size = 1, log = 0;
        while (size < n) size <<= 1, log++;
        dat.assign(2 * size, e());
    }

    SegTree_point_min(vector<S> v) {
        _n = v.size();
        size = 1, log = 0;
        while (size < _n) size <<= 1, log++;
        dat.assign(2 * size, e());
        for (int i = 0; i < _n; i++) dat[size + i] = v[i];
        for (int i = size - 1; i > 0; i--) update(i);
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        dat[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) {
        assert(0 <= p && p < _n);
        return dat[p + size];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        S sml = e(), smr = e();
        l += size, r += size;

        while (l < r) {
            if (l & 1) sml = op(sml, dat[l++]);
            if (r & 1) smr = op(dat[--r], smr);
            l >>= 1, r >>= 1;
        }

        return op(sml, smr);
    }

    S all_prod() { return dat[1]; }

    template <class F> int max_right(int l, F f) const {
        assert(0 <= l && l <= _n);
        assert(f(e()));
        if (l == _n) return _n;
        l += size;
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, dat[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, dat[l]))) {
                        sm = op(sm, dat[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, dat[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <class F> int min_left(int r, F f) const {
        assert(0 <= r && r <= _n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(dat[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(dat[r], sm))) {
                        sm = op(dat[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(dat[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    vector<S> dat;

    static S op(S a, S b) { return min(a, b); } 
    static S e() { return -1; }
    void update(int k) { dat[k] = op(dat[2 * k], dat[2 * k + 1]); }
};