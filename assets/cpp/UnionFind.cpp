struct UnionFind {
    public:
    UnionFind() : _n(0) {}

    UnionFind(int n) {
        init(n);
    }

    void init(int n) {
        _n = n;
        _parent.assign(n, -1);
    }

    int merge(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        int x = leader(a), y = leader(b);
        if (x == y) return x;
        if (-_parent[x] < -_parent[y]) swap(x, y);
        _parent[x] += _parent[y];
        _parent[y] = x;
        return x;
    }

    bool same(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        return leader(a) == leader(b);
    }

    int leader(int a) {
        assert(0 <= a && a < _n);
        if (_parent[a] < 0) return a;
        return _parent[a] = leader(_parent[a]);
    }

    int size(int a) {
        assert(0 <= a && a < _n);
        return -_parent[leader(a)];
    }

    vector<vector<int>> groups() {
        vector<int> leader_buf(_n), group_size(_n);
        for (int i = 0; i < _n; i++) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        vector<vector<int>> result(_n);
        for (int i = 0; i < _n; i++) {
            result[i].reserve(group_size[i]);
        }
        for (int i = 0; i < _n; i++) {
            result[leader_buf[i]].push_back(i);
        }
        result.erase(
            remove_if(result.begin(), result.end(),
                      [&](const vector<int> &v) { return v.empty(); }),
            result.end());
        return result;
    }

    private:
    int _n;
    vector<int> _parent;
};