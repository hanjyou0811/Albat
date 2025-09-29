#include "../library.h"
#include "../libraryflg.h"

void LibraryManager::initBasicSettings()
{
    {
        std::string name = "pragma";
        std::string code = "";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        deps.push_back("Ofast");
        deps.push_back("unroll-loops");
        lib_names.push_back(name);
        lib_Ident[name] = code;
        lib_dependencies[name] = deps;
        lib_positions[name] = pos;
    }

    {
        std::string name = "Ofast";
        std::string code = "#pragma GCC optimize(\"Ofast\")\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(name);
        lib_Ident[name] = code;
        lib_dependencies[name] = deps;
        lib_positions[name] = pos;
    }

    {
        std::string name = "unroll-loops";
        std::string code = "#pragma GCC optimize(\"unroll-loops\")\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(name);
        lib_Ident[name] = code;
        lib_dependencies[name] = deps;
        lib_positions[name] = pos;
    }
    {
        std::string name = "stdc++";
        std::string code = "#include <bits/stdc++.h>\n";
        std::string pos  = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(name);
        lib_Ident[name] = code;
        lib_dependencies[name] = deps;
        lib_positions[name] = pos;
    }

    {
        std::string name = "namespace_std";
        std::string code = "using namespace std;\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(name);
        lib_Ident[name] = code;
        lib_dependencies[name] = deps;
        lib_positions[name] = pos;
    }

    {
        std::string name = "ACL";
        std::string code = "#include <atcoder/all>\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        if (NAMESPACE_ACL)
            deps.push_back("namespace_acl");
        lib_names.push_back(name);
        lib_Ident[name] = code;
        lib_dependencies[name] = deps;
        lib_positions[name] = pos;
    }

    {
        std::string name = "namespace_acl";
        std::string code = "using namespace atcoder;\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(name);
        lib_Ident[name] = code;
        lib_dependencies[name] = deps;
        lib_positions[name] = pos;
    }
    {
        std::string name = "DEFINE_INT_LL";
        std::string code = "#define int long long\n";
        std::string pos = (DEFINE_INT_LL==1) ? MAIN : HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(name);
        lib_Ident[name] = code;
        lib_dependencies[name] = deps;
        lib_positions[name] = pos;
    }  
}

void LibraryManager::initMathLibraries()
{
    {
        std::string func_name = "gcd";
        std::string func_code = "template<class T, class S> inline T GCD_L(T a, S b){T r; while(b) r=a, a=b, b=r%a; return a;}\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "lcm";
        std::string func_code = "template<class T, class U> inline T LCM_L(T a, U b){return a/GCD_L(a,b)*b;}\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        deps.push_back("gcd");
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "_pow";
        std::string func_code = "template<class T, class S> inline T _pow(T a, S b){\n  T res = 1;\n  res = 1;\n  for(;;){\n    if(b&1) res *= a;\n    b >>= 1;\n    if(b==0) break;\n    a *= a;\n  }\n  return res;\n}\ninline double _pow(double a, double b){\n  return pow(a,b);\n}\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }  
    {
        std::string func_name = "_div";
        std::string func_code = "template<class S, class T> inline S _div(S a, T b){ return ( a + b - 1) / b; }\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }  
    {
        std::string func_name = "_mod";
        std::string func_code = "template<class S, class T> inline S _mod(S a, const T b){\n  S x = a;\n x %= b;\n  if(x < 0) x += b;\n  return x;\n}\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "makeprimes";
        std::string func_code = "template<typename T>\nvector<T> makePrimes(T n) { // [2,n]\n    vector<T> res, pr(n + 1, 1);\n    pr[0] = pr[1] = 0;\n    rep(p, 2, sqrt(n) + 2) if (pr[p]) for (int x = p * 2; x <= n; x += p) pr[x] = 0;\n    rep(p, 2, n + 1) if (pr[p]) res.push_back(p);\n    return res;\n}\n";
        std::string pos = HEAD;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
    }
    {
       std::string func_name = "divnum";
       std::string func_code = "template<typename T>\nvector<T> divNum(T n) { \n    vector<T> S;\n    for (int i = 1; 1LL*i*i <= n; i++) if (n%i == 0) { S.push_back(i); if (i*i != n) S.push_back(n / i); }\n    sort(S.begin(), S.end());\n    return S;\n}\n";
       std::string pos = HEAD;
    
       lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos; 
    }
    {
        std::string func_name = "sum";
        std::string func_code = "template<typename T, typename = void>\nstruct is_iterable : false_type { };\ntemplate<typename T>\nstruct is_iterable<T, void_t<\n    decltype(begin(declval<T>())),\n    decltype(end(declval<T>()))\n>> : true_type { };\ntemplate<typename CharT, typename Traits, typename Alloc>\nstruct is_iterable<basic_string<CharT, Traits, Alloc>> : false_type { };\ntemplate<typename Container>\nauto sum(const Container& container)\n    -> enable_if_t<is_iterable<Container>::value, decay_t<decltype(*begin(container))>>\n{\n    using value_type = decay_t<decltype(*begin(container))>;\n    assert(!container.empty() && \"Container should not be empty\");\n    return accumulate(begin(container), end(container), value_type{});\n}\ntemplate<typename T, typename... Args>\nauto sum(T arg, Args... args)\n    -> enable_if_t<!is_iterable<T>::value, decltype((arg + ... + args))>\n{\n    static_assert(sizeof...(Args) > 0, \"At least two arguments are required\");\n    return (arg + ... + args);\n}\n";
        std::string pos = HEAD;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
    }
}

void LibraryManager::initDataStructures()
{
    {
        std::string func_name = "UnionFind";
        std::string func_code = "struct UnionFind {\n    public:\n    UnionFind() : _n(0) {}\n\n    UnionFind(int n) {\n        init(n);\n    }\n\n    void init(int n) {\n        _n = n;\n        _parent.assign(n, -1);\n    }\n\n    int merge(int a, int b) {\n        assert(0 <= a && a < _n);\n        assert(0 <= b && b < _n);\n        int x = leader(a), y = leader(b);\n        if (x == y) return x;\n        if (-_parent[x] < -_parent[y]) swap(x, y);\n        _parent[x] += _parent[y];\n        _parent[y] = x;\n        return x;\n    }\n\n    bool same(int a, int b) {\n        assert(0 <= a && a < _n);\n        assert(0 <= b && b < _n);\n        return leader(a) == leader(b);\n    }\n\n    int leader(int a) {\n        assert(0 <= a && a < _n);\n        if (_parent[a] < 0) return a;\n        return _parent[a] = leader(_parent[a]);\n    }\n\n    int size(int a) {\n        assert(0 <= a && a < _n);\n        return -_parent[leader(a)];\n    }\n\n    vector<vector<int>> groups() {\n        vector<int> leader_buf(_n), group_size(_n);\n        for (int i = 0; i < _n; i++) {\n            leader_buf[i] = leader(i);\n            group_size[leader_buf[i]]++;\n        }\n        vector<vector<int>> result(_n);\n        for (int i = 0; i < _n; i++) {\n            result[i].reserve(group_size[i]);\n        }\n        for (int i = 0; i < _n; i++) {\n            result[leader_buf[i]].push_back(i);\n        }\n        result.erase(\n            remove_if(result.begin(), result.end(),\n                      [&](const vector<int> &v) { return v.empty(); }),\n            result.end());\n        return result;\n    }\n\n    private:\n    int _n;\n    vector<int> _parent;\n};";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "WeightedUnionFind";
        std::string func_code = "template <class T> struct WeightedUnionFind {\n  vector<int> par;\n  vector<int> rank;\n  vector<T> diff_weight;\n\n  WeightedUnionFind(int n = 1, T sum = 0) { init(n, sum); }\n  void init(int n = 1, T sum = 0) {\n    par.resize(n);\n    rank.resize(n);\n    diff_weight.resize(n);\n    for (int i = 0; i < n; i++)\n      par[i] = i, rank[i] = 0, diff_weight[i] = sum;\n  }\n\n  int root(int x) {\n    if (par[x] == x) {\n      return x;\n    } else {\n      int r = root(par[x]);\n      diff_weight[x] += diff_weight[par[x]];\n      return par[x] = r;\n    }\n  }\n\n  T weight(int x) {\n    root(x);\n    return diff_weight[x];\n  }\n\n  bool issame(int x, int y) { return root(x) == root(y); }\n\n  bool merge(int x, int y, T w) {\n    w += weight(x);\n    w -= weight(y);\n    x = root(x);\n    y = root(y);\n    if (x == y)\n      return false;\n    if (rank[x] < rank[y])\n      swap(x, y), w = -w;\n    if (rank[x] == rank[y])\n      ++rank[x];\n    par[y] = x;\n    diff_weight[y] = w;\n    return true;\n  }\n\n  T diff(int x, int y) { return weight(y) - weight(x); }\n};\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "_SegTree";
        std::string func_code = "template <class S, S (*op)(S, S), S (*e)()> struct _SegTree {\n  public:\n    _SegTree(int n) : _n(n) {\n        size = 1, log = 0;\n        while (size < n) size <<= 1, log++;\n        dat.assign(2 * size, e());\n    }\n\n    SegTree(vector<S> v) {\n        _n = v.size();\n        size = 1, log = 0;\n        while (size < _n) size <<= 1, log++;\n        dat.assign(2 * size, e());\n        for (int i = 0; i < _n; i++) dat[size + i] = v[i];\n        for (int i = size - 1; i > 0; i--) update(i);\n    }\n\n    void set(int p, S x) {\n        assert(0 <= p && p < _n);\n        p += size;\n        dat[p] = x;\n        for (int i = 1; i <= log; i++) update(p >> i);\n    }\n\n    S get(int p) {\n        assert(0 <= p && p < _n);\n        return dat[p + size];\n    }\n\n    S prod(int l, int r) {\n        assert(0 <= l && l <= r && r <= _n);\n        S sml = e(), smr = e();\n        l += size, r += size;\n\n        while (l < r) {\n            if (l & 1) sml = op(sml, dat[l++]);\n            if (r & 1) smr = op(dat[--r], smr);\n            l >>= 1, r >>= 1;\n        }\n\n        return op(sml, smr);\n    }\n\n    S all_prod() { return dat[1]; }\n\n    template <bool (*f)(S)> int max_right(int l) const {\n        return max_right(l, [](S x) { return f(x); });\n    }\n    template <class F> int max_right(int l, F f) const {\n        assert(0 <= l && l <= _n);\n        assert(f(e()));\n        if (l == _n) return _n;\n        l += size;\n        S sm = e();\n        do {\n            while (l % 2 == 0) l >>= 1;\n            if (!f(op(sm, dat[l]))) {\n                while (l < size) {\n                    l = (2 * l);\n                    if (f(op(sm, dat[l]))) {\n                        sm = op(sm, dat[l]);\n                        l++;\n                    }\n                }\n                return l - size;\n            }\n            sm = op(sm, dat[l]);\n            l++;\n        } while ((l & -l) != l);\n        return _n;\n    }\n\n    template <bool (*f)(S)> int min_left(int r) const {\n        return min_left(r, [](S x) { return f(x); });\n    }\n    template <class F> int min_left(int r, F f) const {\n        assert(0 <= r && r <= _n);\n        assert(f(e()));\n        if (r == 0) return 0;\n        r += size;\n        S sm = e();\n        do {\n            r--;\n            while (r > 1 && (r % 2)) r >>= 1;\n            if (!f(op(dat[r], sm))) {\n                while (r < size) {\n                    r = (2 * r + 1);\n                    if (f(op(dat[r], sm))) {\n                        sm = op(dat[r], sm);\n                        r--;\n                    }\n                }\n                return r + 1 - size;\n            }\n            sm = op(dat[r], sm);\n        } while ((r & -r) != r);\n        return 0;\n    }\n\n  private:\n    int _n, size, log;\n    vector<S> dat;\n\n    void update(int k) { dat[k] = op(dat[2 * k], dat[2 * k + 1]); }\n};";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "SegTree_pt_max";
        std::string func_code = "template <class S> struct SegTree_pt_max {\n  public:\n    SegTree_pt_max(int n) : _n(n) {\n        size = 1, log = 0;\n        while (size < n) size <<= 1, log++;\n        dat.assign(2 * size, e());\n    }\n\n    SegTree_point_max(vector<S> v) {\n        _n = v.size();\n        size = 1, log = 0;\n        while (size < _n) size <<= 1, log++;\n        dat.assign(2 * size, e());\n        for (int i = 0; i < _n; i++) dat[size + i] = v[i];\n        for (int i = size - 1; i > 0; i--) update(i);\n    }\n\n    void set(int p, S x) {\n        assert(0 <= p && p < _n);\n        p += size;\n        dat[p] = x;\n        for (int i = 1; i <= log; i++) update(p >> i);\n    }\n\n    S get(int p) {\n        assert(0 <= p && p < _n);\n        return dat[p + size];\n    }\n\n    S prod(int l, int r) {\n        assert(0 <= l && l <= r && r <= _n);\n        S sml = e(), smr = e();\n        l += size, r += size;\n\n        while (l < r) {\n            if (l & 1) sml = op(sml, dat[l++]);\n            if (r & 1) smr = op(dat[--r], smr);\n            l >>= 1, r >>= 1;\n        }\n\n        return op(sml, smr);\n    }\n\n    S all_prod() { return dat[1]; }\n\n    template <class F> int max_right(int l, F f) const {\n        assert(0 <= l && l <= _n);\n        assert(f(e()));\n        if (l == _n) return _n;\n        l += size;\n        S sm = e();\n        do {\n            while (l % 2 == 0) l >>= 1;\n            if (!f(op(sm, dat[l]))) {\n                while (l < size) {\n                    l = (2 * l);\n                    if (f(op(sm, dat[l]))) {\n                        sm = op(sm, dat[l]);\n                        l++;\n                    }\n                }\n                return l - size;\n            }\n            sm = op(sm, dat[l]);\n            l++;\n        } while ((l & -l) != l);\n        return _n;\n    }\n\n    template <class F> int min_left(int r, F f) const {\n        assert(0 <= r && r <= _n);\n        assert(f(e()));\n        if (r == 0) return 0;\n        r += size;\n        S sm = e();\n        do {\n            r--;\n            while (r > 1 && (r % 2)) r >>= 1;\n            if (!f(op(dat[r], sm))) {\n                while (r < size) {\n                    r = (2 * r + 1);\n                    if (f(op(dat[r], sm))) {\n                        sm = op(dat[r], sm);\n                        r--;\n                    }\n                }\n                return r + 1 - size;\n            }\n            sm = op(dat[r], sm);\n        } while ((r & -r) != r);\n        return 0;\n    }\n\n  private:\n    int _n, size, log;\n    vector<S> dat;\n\n    static S op(S a, S b) { return max(a, b); } \n    static S e() { return -1; }\n    void update(int k) { dat[k] = op(dat[2 * k], dat[2 * k + 1]); }\n};";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "SegTree_pt_min";
        std::string func_code = "template <class S> struct SegTree_pt_min {\n  public:\n    SegTree_pt_min(int n) : _n(n) {\n        size = 1, log = 0;\n        while (size < n) size <<= 1, log++;\n        dat.assign(2 * size, e());\n    }\n\n    SegTree_point_min(vector<S> v) {\n        _n = v.size();\n        size = 1, log = 0;\n        while (size < _n) size <<= 1, log++;\n        dat.assign(2 * size, e());\n        for (int i = 0; i < _n; i++) dat[size + i] = v[i];\n        for (int i = size - 1; i > 0; i--) update(i);\n    }\n\n    void set(int p, S x) {\n        assert(0 <= p && p < _n);\n        p += size;\n        dat[p] = x;\n        for (int i = 1; i <= log; i++) update(p >> i);\n    }\n\n    S get(int p) {\n        assert(0 <= p && p < _n);\n        return dat[p + size];\n    }\n\n    S prod(int l, int r) {\n        assert(0 <= l && l <= r && r <= _n);\n        S sml = e(), smr = e();\n        l += size, r += size;\n\n        while (l < r) {\n            if (l & 1) sml = op(sml, dat[l++]);\n            if (r & 1) smr = op(dat[--r], smr);\n            l >>= 1, r >>= 1;\n        }\n\n        return op(sml, smr);\n    }\n\n    S all_prod() { return dat[1]; }\n\n    template <class F> int max_right(int l, F f) const {\n        assert(0 <= l && l <= _n);\n        assert(f(e()));\n        if (l == _n) return _n;\n        l += size;\n        S sm = e();\n        do {\n            while (l % 2 == 0) l >>= 1;\n            if (!f(op(sm, dat[l]))) {\n                while (l < size) {\n                    l = (2 * l);\n                    if (f(op(sm, dat[l]))) {\n                        sm = op(sm, dat[l]);\n                        l++;\n                    }\n                }\n                return l - size;\n            }\n            sm = op(sm, dat[l]);\n            l++;\n        } while ((l & -l) != l);\n        return _n;\n    }\n\n    template <class F> int min_left(int r, F f) const {\n        assert(0 <= r && r <= _n);\n        assert(f(e()));\n        if (r == 0) return 0;\n        r += size;\n        S sm = e();\n        do {\n            r--;\n            while (r > 1 && (r % 2)) r >>= 1;\n            if (!f(op(dat[r], sm))) {\n                while (r < size) {\n                    r = (2 * r + 1);\n                    if (f(op(dat[r], sm))) {\n                        sm = op(dat[r], sm);\n                        r--;\n                    }\n                }\n                return r + 1 - size;\n            }\n            sm = op(dat[r], sm);\n        } while ((r & -r) != r);\n        return 0;\n    }\n\n  private:\n    int _n, size, log;\n    vector<S> dat;\n\n    static S op(S a, S b) { return min(a, b); } \n    static S e() { return -1; }\n    void update(int k) { dat[k] = op(dat[2 * k], dat[2 * k + 1]); }\n};";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "SegTree_pt_sum";
        std::string func_code = "template <class S> struct SegTree_pt_sum {\n  public:\n    SegTree_pt_sum(int n) : _n(n) {\n        size = 1, log = 0;\n        while (size < n) size <<= 1, log++;\n        dat.assign(2 * size, e());\n    }\n\n    SegTree_pt_sum(vector<S> v) {\n        _n = v.size();\n        size = 1, log = 0;\n        while (size < _n) size <<= 1, log++;\n        dat.assign(2 * size, e());\n        for (int i = 0; i < _n; i++) dat[size + i] = v[i];\n        for (int i = size - 1; i > 0; i--) update(i);\n    }\n\n    void set(int p, S x) {\n        assert(0 <= p && p < _n);\n        p += size;\n        dat[p] = x;\n        for (int i = 1; i <= log; i++) update(p >> i);\n    }\n\n    S get(int p) {\n        assert(0 <= p && p < _n);\n        return dat[p + size];\n    }\n\n    S prod(int l, int r) {\n        assert(0 <= l && l <= r && r <= _n);\n        S sml = e(), smr = e();\n        l += size, r += size;\n\n        while (l < r) {\n            if (l & 1) sml = op(sml, dat[l++]);\n            if (r & 1) smr = op(dat[--r], smr);\n            l >>= 1, r >>= 1;\n        }\n\n        return op(sml, smr);\n    }\n\n    S all_prod() { return dat[1]; }\n\n    template <class F> int max_right(int l, F f) const {\n        assert(0 <= l && l <= _n);\n        assert(f(e()));\n        if (l == _n) return _n;\n        l += size;\n        S sm = e();\n        do {\n            while (l % 2 == 0) l >>= 1;\n            if (!f(op(sm, dat[l]))) {\n                while (l < size) {\n                    l = (2 * l);\n                    if (f(op(sm, dat[l]))) {\n                        sm = op(sm, dat[l]);\n                        l++;\n                    }\n                }\n                return l - size;\n            }\n            sm = op(sm, dat[l]);\n            l++;\n        } while ((l & -l) != l);\n        return _n;\n    }\n\n    template <class F> int min_left(int r, F f) const {\n        assert(0 <= r && r <= _n);\n        assert(f(e()));\n        if (r == 0) return 0;\n        r += size;\n        S sm = e();\n        do {\n            r--;\n            while (r > 1 && (r % 2)) r >>= 1;\n            if (!f(op(dat[r], sm))) {\n                while (r < size) {\n                    r = (2 * r + 1);\n                    if (f(op(dat[r], sm))) {\n                        sm = op(dat[r], sm);\n                        r--;\n                    }\n                }\n                return r + 1 - size;\n            }\n            sm = op(dat[r], sm);\n        } while ((r & -r) != r);\n        return 0;\n    }\n\n  private:\n    int _n, size, log;\n    vector<S> dat;\n\n    static S op(S a, S b) { return a + b; } \n    static S e() { return 0; }\n    void update(int k) { dat[k] = op(dat[2 * k], dat[2 * k + 1]); }\n};";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "Fenwick_tree";
        std::string func_code = "template <class T> struct Fenwick_tree {\n  int n;\n  vector<T> bit;\n\n  Fenwick_tree(int n_) : n(n_), bit(n + 1) {}\n\n  void add(int pos, T x)\n  {\n    pos++;\nwhile(pos <= n) {\n      bit[pos] += x;\n      pos += pos & -pos;\n    }\n  }\n\n  T sum(int pos)\n  {\n    T res = 0;\n    while(pos)\n    {\n      res += bit[pos];\n      pos -= pos & -pos;\n    }\n    return res;\n  }\n  // [l, r)の合計\n  T sum(int l, int r)\n  {\n    return sum(r) - sum(l);\n  }\n};";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "DEFINE_MOD";
        std::string func_code = "#define mod 1000000009\n";
        std::string pos = HEAD;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
    }
    {
        //modの変更が効くやつ
        std::string func_name = "mint";
        std::string func_code = "struct mint{\n    static unsigned md;\n    static unsigned W;\n    static unsigned R;\n    static unsigned Rinv;\n    static unsigned mntnum;\n    static unsigned RR;\n    unsigned val;\n    mint():val(0){\n    }\n    mint(int a):val(mulR(a)){\n    }\n    mint(unsigned a):val(mulR(a)){\n    }\n    mint(long long a):val(mulR(a)){\n    }\n    mint(unsigned long long a):val(mulR(a)){\n    }\n    unsigned mulR(unsigned a){\n        return (unsigned long long)a*R%md;\n    }\n    unsigned mulR(int a){\n        if(a < 0){\n            a = a%((int)md)+(int)md;\n        }\n        return mulR((unsigned)a);\n    }\n    unsigned mulR(unsigned long long a){\n        return mulR((unsigned)(a%md));\n    }\n    unsigned mulR(long long a){\n        a %= (int)md;\n        if(a < 0){\n            a += md;\n        }\n        return mulR((unsigned)a);\n    }\n    unsigned reduce(unsigned T){\n        unsigned m=T*mntnum;\n        unsigned t=(T+(unsigned long long)m*md)>>W;\n        if(t>=md){\n            t-=md;\n        }\n        return t;\n    }\n    unsigned reduce(unsigned long long T){\n        unsigned m=T*mntnum;\n        unsigned t=(T+(unsigned long long)m*md)>>W;\n        if(t>=md){\n            t-=md;\n        }\n        return t;\n    }\n    unsigned get(){\n        return reduce(val);\n    }\n    int get_inv(long long a, int md){\n        long long t = a;\n        long long s = md;\n        long long u = 1;\n        long long v = 0;\n        long long e;\n        while (s){\n            e = t / s;\n            t = t - e * s;\n            swap(t, s);\n            u = u - e * v;\n            swap(u, v);\n        }\n        if(u < 0){\n            u += md;\n        }\n        return u;\n    }\n    void set(unsigned MOD){\n        int i;\n        unsigned t;\n        md = MOD;\n        W = 32;\n        R = (1ULL << W) % md;\n        RR = (unsigned long long) R * R % md;\n        if(MOD == 1000000007){\n            Rinv = 518424770;\n            mntnum = 2226617417U;\n        }\n        else if(MOD == 998244353){\n            Rinv = 232013824;\n            mntnum = 998244351;\n        }\n        else if(MOD == 1000000009){\n            Rinv = 171601999;\n            mntnum = 737024967;\n        }\n        else if(MOD == 1004535809){\n            Rinv = 234947584;\n            mntnum = 1004535807;\n        }\n        else if(MOD == 1051721729){\n            Rinv = 257538304;\n            mntnum = 1051721727;\n        }\n        else{\n            Rinv = get_inv(R, MOD);\n            mntnum = 0;\n            t = 0;\n            for(i=0;i<32;i++){\n                if(t % 2 == 0){\n                    t += md;\n                    mntnum |= (1U << i);\n                }\n                t >>= 1;\n            }\n        }\n    }\n    inline mint &operator++(){\n        (*this) += 1;\n        return *this;\n    }\n    inline mint &operator--(){\n        (*this) -= 1;\n        return *this;\n    }\n    inline mint operator++(int){\n        mint res=*this;\n        (*this) += 1;\n        return res;\n    }\n    inline mint operator--(int){\n        mint res=*this;\n        (*this) -= 1;\n        return res;\n    }\n    mint &operator+=(mint a){\n        if(val < a.val){\n            val = val - a.val + md;\n        }\n        else{\n            val -= a.val;\n        }\n        return *this;\n    }\n    mint &operator-=(mint a){\n        if(val < a.val){\n            val = val + md - a.val;\n        }\n        else{\n            val -= a.val;\n        }\n        return *this;\n    }\n    mint &operator*=(mint a){\n        val = reduce((unsigned long long)val * a.val);\n        return *this;\n    }\n    mint &operator/=(mint a){\n        return *this *= a.inverse();\n    }\n    mint operator+(mint a){\n        return mint(*this) += a;\n    }\n    mint operator-(mint a){\n        return mint(*this) -= a;\n    }\n    mint operator*(mint a){\n        return mint(*this) *= a;\n    }\n    mint operator/(mint a){\n        return mint(*this) /= a;\n    }\n    mint operator+(int a){\n        return mint(*this) += mint(a);\n    }\n    mint operator-(int a){\n        return mint(*this) -= mint(a);\n    }\n    mint operator*(int a){\n        return mint(*this) *= mint(a);\n    }\n    mint operator/(int a){\n        return mint(*this) /= mint(a);\n    }\n    mint operator+(long long a){\n        return mint(*this) += mint(a);\n    }\n    mint operator-(long long a){\n        return mint(*this) -= mint(a);\n    }\n    mint operator*(long long a){\n        return mint(*this) *= mint(a);\n    }\n    mint operator/(long long a){\n        return mint(*this) /= mint(a);\n    }\n    mint operator-(void){\n        mint res;\n        if(val){\n            res.val = md - val;\n        }\n        else{\n            res.val = 0;\n        }\n        return res;\n    }\n    operator bool(void){\n        return val != 0;\n    }\n    operator int(void){\n        return get();\n    }\n    operator long long(void){\n        return get();\n    }\n    mint inverse(){\n        int a = val;\n        int b = md;\n        int u = 1;\n        int v = 0;\n        int t;\n        mint res;\n        while(b){\n            t = a / b;\n            a -= t * b;\n            swap(a, b);\n            u -= t * v;\n            swap(u, v);\n        }\n        if(u < 0){\n            u += md;\n        }\n        res.val = (unsigned)u*RR % md;\n        return res;\n    }\n    mint pw(unsigned long long b){\n        mint a = *this;\n        mint res;\n        res.val = R;\n        while(b){\n            if(b & 1){\n                res *= a;\n            }\n            a *= a;\n            b >>= 1;\n        }\n        return res;\n    }\n    mint pw(int b){\n        return pw((unsigned long long)b);\n    }\n    mint pw(long long b){\n        return pw((unsigned long long)b);\n    }\n    bool operator==(int a){\n        return mulR(a) == val;\n    }\n    bool operator==(unsigned a){\n        return mulR(a) == val;\n    }\n    bool operator==(long long a){\n        return mulR(a) == val;\n    }\n    bool operator!=(int a){\n        return mulR(a) != val;\n    }\n    bool operator!=(unsigned a){\n        return mulR(a) != val;\n    }\n    bool operator!=(long long a){\n        return mulR(a) != val;\n    }\n}\n;\nunsigned mint::md;\nunsigned mint::W;\nunsigned mint::R;\nunsigned mint::Rinv;\nunsigned mint::mntnum;\nunsigned mint::RR;\nmint operator+(int a, mint b){\n  return mint(a)+=b;\n}\nmint operator-(int a, mint b){\n  return mint(a)-=b;\n}\nmint operator*(int a, mint b){\n  return mint(a)*=b;\n}\nmint operator/(int a, mint b){\n  return mint(a)/=b;\n}\nmint operator+(long long a, mint b){\n  return mint(a)+=b;\n}\nmint operator-(long long a, mint b){\n  return mint(a)-=b;\n}\nmint operator*(long long a, mint b){\n  return mint(a)*=b;\n}\nmint operator/(long long a, mint b){\n  return mint(a)/=b;\n}\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        deps.push_back("mint_init");
        deps.push_back("DEFINE_MOD");
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "mint_init";
        std::string func_code = "{mint x; x.set(mod);}\n";
        std::string pos = MAIN;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "modint";
        std::string func_code = "struct modint{\n    static unsigned md;\n    static unsigned W;\n    static unsigned R;\n    static unsigned Rinv;\n    static unsigned mntnum;\n    static unsigned RR;\n    unsigned val;\n    modint():val(0){\n    }\n    modint(int a):val(mulR(a)){\n    }\n    modint(unsigned a):val(mulR(a)){\n    }\n    modint(long long a):val(mulR(a)){\n    }\n    modint(unsigned long long a):val(mulR(a)){\n    }\n    unsigned mulR(unsigned a){\n        return (unsigned long long)a*R%md;\n    }\n    unsigned mulR(int a){\n        if(a < 0){\n            a = a%((int)md)+(int)md;\n        }\n        return mulR((unsigned)a);\n    }\n    unsigned mulR(unsigned long long a){\n        return mulR((unsigned)(a%md));\n    }\n    unsigned mulR(long long a){\n        a %= (int)md;\n        if(a < 0){\n            a += md;\n        }\n        return mulR((unsigned)a);\n    }\n    unsigned reduce(unsigned T){\n        unsigned m=T*mntnum;\n        unsigned t=(T+(unsigned long long)m*md)>>W;\n        if(t>=md){\n            t-=md;\n        }\n        return t;\n    }\n    unsigned reduce(unsigned long long T){\n        unsigned m=T*mntnum;\n        unsigned t=(T+(unsigned long long)m*md)>>W;\n        if(t>=md){\n            t-=md;\n        }\n        return t;\n    }\n    unsigned get(){\n        return reduce(val);\n    }\n    int get_inv(long long a, int md){\n        long long t = a;\n        long long s = md;\n        long long u = 1;\n        long long v = 0;\n        long long e;\n        while (s){\n            e = t / s;\n            t = t - e * s;\n            swap(t, s);\n            u = u - e * v;\n            swap(u, v);\n        }\n        if(u < 0){\n            u += md;\n        }\n        return u;\n    }\n    void set(unsigned MOD){\n        int i;\n        unsigned t;\n        md = MOD;\n        W = 32;\n        R = (1ULL << W) % md;\n        RR = (unsigned long long) R * R % md;\n        if(MOD == 1000000007){\n            Rinv = 518424770;\n            mntnum = 2226617417U;\n        }\n        else if(MOD == 998244353){\n            Rinv = 232013824;\n            mntnum = 998244351;\n        }\n        else if(MOD == 1000000009){\n            Rinv = 171601999;\n            mntnum = 737024967;\n        }\n        else if(MOD == 1004535809){\n            Rinv = 234947584;\n            mntnum = 1004535807;\n        }\n        else if(MOD == 1051721729){\n            Rinv = 257538304;\n            mntnum = 1051721727;\n        }\n        else{\n            Rinv = get_inv(R, MOD);\n            mntnum = 0;\n            t = 0;\n            for(i=0;i<32;i++){\n                if(t % 2 == 0){\n                    t += md;\n                    mntnum |= (1U << i);\n                }\n                t >>= 1;\n            }\n        }\n    }\n    inline modint &operator++(){\n        (*this) += 1;\n        return *this;\n    }\n    inline modint &operator--(){\n        (*this) -= 1;\n        return *this;\n    }\n    inline modint operator++(int){\n        modint res=*this;\n        (*this) += 1;\n        return res;\n    }\n    inline modint operator--(int){\n        modint res=*this;\n        (*this) -= 1;\n        return res;\n    }\n    modint &operator+=(modint a){\n        if(val < a.val){\n            val = val - a.val + md;\n        }\n        else{\n            val -= a.val;\n        }\n        return *this;\n    }\n    modint &operator-=(modint a){\n        if(val < a.val){\n            val = val + md - a.val;\n        }\n        else{\n            val -= a.val;\n        }\n        return *this;\n    }\n    modint &operator*=(modint a){\n        val = reduce((unsigned long long)val * a.val);\n        return *this;\n    }\n    modint &operator/=(modint a){\n        return *this *= a.inverse();\n    }\n    modint operator+(modint a){\n        return modint(*this) += a;\n    }\n    modint operator-(modint a){\n        return modint(*this) -= a;\n    }\n    modint operator*(modint a){\n        return modint(*this) *= a;\n    }\n    modint operator/(modint a){\n        return modint(*this) /= a;\n    }\n    modint operator+(int a){\n        return modint(*this) += modint(a);\n    }\n    modint operator-(int a){\n        return modint(*this) -= modint(a);\n    }\n    modint operator*(int a){\n        return modint(*this) *= modint(a);\n    }\n    modint operator/(int a){\n        return modint(*this) /= modint(a);\n    }\n    modint operator+(long long a){\n        return modint(*this) += modint(a);\n    }\n    modint operator-(long long a){\n        return modint(*this) -= modint(a);\n    }\n    modint operator*(long long a){\n        return modint(*this) *= modint(a);\n    }\n    modint operator/(long long a){\n        return modint(*this) /= modint(a);\n    }\n    modint operator-(void){\n        modint res;\n        if(val){\n            res.val = md - val;\n        }\n        else{\n            res.val = 0;\n        }\n        return res;\n    }\n    operator bool(void){\n        return val != 0;\n    }\n    operator int(void){\n        return get();\n    }\n    operator long long(void){\n        return get();\n    }\n    modint inverse(){\n        int a = val;\n        int b = md;\n        int u = 1;\n        int v = 0;\n        int t;\n        modint res;\n        while(b){\n            t = a / b;\n            a -= t * b;\n            swap(a, b);\n            u -= t * v;\n            swap(u, v);\n        }\n        if(u < 0){\n            u += md;\n        }\n        res.val = (unsigned)u*RR % md;\n        return res;\n    }\n    modint pw(unsigned long long b){\n        modint a = *this;\n        modint res;\n        res.val = R;\n        while(b){\n            if(b & 1){\n                res *= a;\n            }\n            a *= a;\n            b >>= 1;\n        }\n        return res;\n    }\n    modint pw(int b){\n        return pw((unsigned long long)b);\n    }\n    modint pw(long long b){\n        return pw((unsigned long long)b);\n    }\n    bool operator==(int a){\n        return mulR(a) == val;\n    }\n    bool operator==(unsigned a){\n        return mulR(a) == val;\n    }\n    bool operator==(long long a){\n        return mulR(a) == val;\n    }\n    bool operator!=(int a){\n        return mulR(a) != val;\n    }\n    bool operator!=(unsigned a){\n        return mulR(a) != val;\n    }\n    bool operator!=(long long a){\n        return mulR(a) != val;\n    }\n}\n;\nunsigned modint::md;\nunsigned modint::W;\nunsigned modint::R;\nunsigned modint::Rinv;\nunsigned modint::mntnum;\nunsigned modint::RR;\nmodint operator+(int a, modint b){\n    return modint(a)+=b;\n}\nmodint operator-(int a, modint b){\n    return modint(a)-=b;\n}\nmodint operator*(int a, modint b){\n    return modint(a)*=b;\n}\nmodint operator/(int a, modint b){\n    return modint(a)/=b;\n}\nmodint operator+(long long a, modint b){\n    return modint(a)+=b;\n}\nmodint operator-(long long a, modint b){\n    return modint(a)-=b;\n}\nmodint operator*(long long a, modint b){\n    return modint(a)*=b;\n}\nmodint operator/(long long a, modint b){\n    return modint(a)/=b;\n}\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        deps.push_back("modint_init");
        deps.push_back("DEFINE_MOD");
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }  
    {
        std::string func_name = "modint_init";
        std::string func_code = "{modint x; x.set(mod);}\n";
        std::string pos = MAIN;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "mod_int";
        std::string func_code = "struct mod_int{\n    static unsigned md;\n    static unsigned W;\n    static unsigned R;\n    static unsigned Rinv;\n    static unsigned mntnum;\n    static unsigned RR;\n    unsigned val;\n    mod_int():val(0){\n    }\n    mod_int(int a):val(mulR(a)){\n    }\n    mod_int(unsigned a):val(mulR(a)){\n    }\n    mod_int(long long a):val(mulR(a)){\n    }\n    mod_int(unsigned long long a):val(mulR(a)){\n    }\n    unsigned mulR(unsigned a){\n        return (unsigned long long)a*R%md;\n    }\n    unsigned mulR(int a){\n        if(a < 0){\n            a = a%((int)md)+(int)md;\n        }\n        return mulR((unsigned)a);\n    }\n    unsigned mulR(unsigned long long a){\n        return mulR((unsigned)(a%md));\n    }\n    unsigned mulR(long long a){\n        a %= (int)md;\n        if(a < 0){\n            a += md;\n        }\n        return mulR((unsigned)a);\n    }\n    unsigned reduce(unsigned T){\n        unsigned m=T*mntnum;\n        unsigned t=(T+(unsigned long long)m*md)>>W;\n        if(t>=md){\n            t-=md;\n        }\n        return t;\n    }\n    unsigned reduce(unsigned long long T){\n        unsigned m=T*mntnum;\n        unsigned t=(T+(unsigned long long)m*md)>>W;\n        if(t>=md){\n            t-=md;\n        }\n        return t;\n    }\n    unsigned get(){\n        return reduce(val);\n    }\n    int get_inv(long long a, int md){\n        long long t = a;\n        long long s = md;\n        long long u = 1;\n        long long v = 0;\n        long long e;\n        while (s){\n            e = t / s;\n            t = t - e * s;\n            swap(t, s);\n            u = u - e * v;\n            swap(u, v);\n        }\n        if(u < 0){\n            u += md;\n        }\n        return u;\n    }\n    void set(unsigned MOD){\n        int i;\n        unsigned t;\n        md = MOD;\n        W = 32;\n        R = (1ULL << W) % md;\n        RR = (unsigned long long) R * R % md;\n        if(MOD == 1000000007){\n            Rinv = 518424770;\n            mntnum = 2226617417U;\n        }\n        else if(MOD == 998244353){\n            Rinv = 232013824;\n            mntnum = 998244351;\n        }\n        else if(MOD == 1000000009){\n            Rinv = 171601999;\n            mntnum = 737024967;\n        }\n        else if(MOD == 1004535809){\n            Rinv = 234947584;\n            mntnum = 1004535807;\n        }\n        else if(MOD == 1051721729){\n            Rinv = 257538304;\n            mntnum = 1051721727;\n        }\n        else{\n            Rinv = get_inv(R, MOD);\n            mntnum = 0;\n            t = 0;\n            for(i=0;i<32;i++){\n                if(t % 2 == 0){\n                    t += md;\n                    mntnum |= (1U << i);\n                }\n                t >>= 1;\n            }\n        }\n    }\n    inline mod_int &operator++(){\n        (*this) += 1;\n        return *this;\n    }\n    inline mod_int &operator--(){\n        (*this) -= 1;\n        return *this;\n    }\n    inline mod_int operator++(int){\n        mod_int res=*this;\n        (*this) += 1;\n        return res;\n    }\n    inline mod_int operator--(int){\n        mod_int res=*this;\n        (*this) -= 1;\n        return res;\n    }\n    mod_int &operator+=(mod_int a){\n        if(val < a.val){\n            val = val - a.val + md;\n        }\n        else{\n            val -= a.val;\n        }\n        return *this;\n    }\n    mod_int &operator-=(mod_int a){\n        if(val < a.val){\n            val = val + md - a.val;\n        }\n        else{\n            val -= a.val;\n        }\n        return *this;\n    }\n    mod_int &operator*=(mod_int a){\n        val = reduce((unsigned long long)val * a.val);\n        return *this;\n    }\n    mod_int &operator/=(mod_int a){\n        return *this *= a.inverse();\n    }\n    mod_int operator+(mod_int a){\n        return mod_int(*this) += a;\n    }\n    mod_int operator-(mod_int a){\n        return mod_int(*this) -= a;\n    }\n    mod_int operator*(mod_int a){\n        return mod_int(*this) *= a;\n    }\n    mod_int operator/(mod_int a){\n        return mod_int(*this) /= a;\n    }\n    mod_int operator+(int a){\n        return mod_int(*this) += mod_int(a);\n    }\n    mod_int operator-(int a){\n        return mod_int(*this) -= mod_int(a);\n    }\n    mod_int operator*(int a){\n        return mod_int(*this) *= mod_int(a);\n    }\n    mod_int operator/(int a){\n        return mod_int(*this) /= mod_int(a);\n    }\n    mod_int operator+(long long a){\n        return mod_int(*this) += mod_int(a);\n    }\n    mod_int operator-(long long a){\n        return mod_int(*this) -= mod_int(a);\n    }\n    mod_int operator*(long long a){\n        return mod_int(*this) *= mod_int(a);\n    }\n    mod_int operator/(long long a){\n        return mod_int(*this) /= mod_int(a);\n    }\n    mod_int operator-(void){\n        mod_int res;\n        if(val){\n            res.val = md - val;\n        }\n        else{\n            res.val = 0;\n        }\n        return res;\n    }\n    operator bool(void){\n        return val != 0;\n    }\n    operator int(void){\n        return get();\n    }\n    operator long long(void){\n        return get();\n    }\n    mod_int inverse(){\n        int a = val;\n        int b = md;\n        int u = 1;\n        int v = 0;\n        int t;\n        mod_int res;\n        while(b){\n            t = a / b;\n            a -= t * b;\n            swap(a, b);\n            u -= t * v;\n            swap(u, v);\n        }\n        if(u < 0){\n            u += md;\n        }\n        res.val = (unsigned)u*RR % md;\n        return res;\n    }\n    mod_int pw(unsigned long long b){\n        mod_int a = *this;\n        mod_int res;\n        res.val = R;\n        while(b){\n            if(b & 1){\n                res *= a;\n            }\n            a *= a;\n            b >>= 1;\n        }\n        return res;\n    }\n    mod_int pw(int b){\n        return pw((unsigned long long)b);\n    }\n    mod_int pw(long long b){\n        return pw((unsigned long long)b);\n    }\n    bool operator==(int a){\n        return mulR(a) == val;\n    }\n    bool operator==(unsigned a){\n        return mulR(a) == val;\n    }\n    bool operator==(long long a){\n        return mulR(a) == val;\n    }\n    bool operator!=(int a){\n        return mulR(a) != val;\n    }\n    bool operator!=(unsigned a){\n        return mulR(a) != val;\n    }\n    bool operator!=(long long a){\n        return mulR(a) != val;\n    }\n}\n;\nunsigned mod_int::md;\nunsigned mod_int::W;\nunsigned mod_int::R;\nunsigned mod_int::Rinv;\nunsigned mod_int::mntnum;\nunsigned mod_int::RR;\nmod_int operator+(int a, mod_int b){\n    return mod_int(a)+=b;\n}\nmod_int operator-(int a, mod_int b){\n    return mod_int(a)-=b;\n}\nmod_int operator*(int a, mod_int b){\n    return mod_int(a)*=b;\n}\nmod_int operator/(int a, mod_int b){\n    return mod_int(a)/=b;\n}\nmod_int operator+(long long a, mod_int b){\n    return mod_int(a)+=b;\n}\nmod_int operator-(long long a, mod_int b){\n    return mod_int(a)-=b;\n}\nmod_int operator*(long long a, mod_int b){\n    return mod_int(a)*=b;\n}\nmod_int operator/(long long a, mod_int b){\n    return mod_int(a)/=b;\n}\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        deps.push_back("mod_int_init");
        deps.push_back("DEFINE_MOD");
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }  
    {
        std::string func_name = "mod_int_init";
        std::string func_code = "{mod_int x; x.set(mod);}\n";
        std::string pos = MAIN;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "MexSet";
        std::string func_code = "struct MexSet {\n    set<pair<int, int>> s;\n    MexSet() {\n      s.emplace(INT_MIN, INT_MIN);\n      s.emplace(INT_MAX, INT_MAX);\n    }\n    bool contains(int x) {\n      auto it = prev(s.lower_bound(make_pair(x + 1, x + 1)));\n      auto [l, u] = *it;\n      return l <= x && x <= u;\n    }\n    bool insert(int x) {\n      auto nit = s.lower_bound(make_pair(x + 1, x + 1));\n      auto it = prev(nit);\n      auto [l, u] = *it;\n      auto [nl, nu] = *nit;\n      if (l <= x && x <= u)\n        return false;\n      if (u == x - 1) {\n        if (nl == x + 1) {\n          s.erase(it);\n          s.erase(nit);\n          s.emplace(l, nu);\n        } else {\n          s.erase(it);\n          s.emplace(l, x);\n        }\n      } else {\n        if (nl == x + 1) {\n          s.erase(nit);\n          s.emplace(x, nu);\n        } else {\n          s.emplace(x, x);\n        }\n      }\n      return true;\n    }\n    int mex(int x = 0) {\n      auto [l, u] = *prev(s.lower_bound(make_pair(x + 1, x + 1)));\n      if (l <= x && x <= u) {\n        return u + 1;\n      } else {\n        return x;\n      }\n    }\n};\n"; 
        std::string pos = HEAD;
        
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
    }
}

void LibraryManager::initUtilityFunctions(){
    {
        std::string func_name = "FastIO";
        std::string func_code = "class FastIO {\n    static const int read_siz = (1 << 28), write_siz = (1 << 28);\n    char read_buf[read_siz], write_buf[write_siz], *rb, *wb;\n    char tmp_s[20];\n\n    #define wc(x) *(wb++) = x\n    public:\n    FastIO() {\n        fread(read_buf, 1, read_siz, stdin);\n        rb = read_buf;\n        wb = write_buf;\n    }\n    ~FastIO() { fwrite(write_buf, 1, wb - write_buf, stdout); }\n\n    inline void read(double &x) {\n        bool neg = false;\n        x = 0;\n        double factor = 1.0;\n        \n        while ((*rb < \'0\' || *rb > \'9\') && *rb != \'-\' && *rb != \'.\')\n            ++rb;\n        \n        if (*rb == \'-\') {\n            neg = true;\n            ++rb;\n        }\n\n        while (\'0\' <= *rb && *rb <= \'9\') {\n            x = x * 10 + (*rb - \'0\');\n            ++rb;\n        }\n\n        if (*rb == \'.\') {\n            ++rb;\n            while (\'0\' <= *rb && *rb <= \'9\') {\n                factor *= 0.1;\n                x += (*rb - \'0\') * factor;\n                ++rb;\n            }\n        }\n\n        if (neg)\n            x = -x;\n    }\n\n    template <typename T> inline void read(T &x) {\n        bool neg = false;\n        x = 0;\n        while ((*rb < \'0\' || *rb > \'9\') && *rb != \'-\') ++rb;\n        if (*rb == \'-\') {\n            neg = true;\n            ++rb;\n        }\n        while (\'0\' <= *rb && *rb <= \'9\') {\n            x = 10 * x + (*rb - \'0\');\n            ++rb;\n        }\n        if (neg) x = -x;\n    }\n\n    inline void read(string &s) {\n        s.clear();\n        while (*rb <= \' \') ++rb;\n        while (*rb > \' \') {\n            s += *(rb++);\n        }\n    }\n\n    inline void read(char &c) {\n        while (*rb <= \' \') ++rb;\n        c = *(rb++);\n    }\n\n    inline void wt_ln(double x, string sep = \"\\n\") {\n        if (x < 0) {\n            wc(\'-\');\n            x = -x;\n        }\n\n        long long int_part = static_cast<long long>(x);\n        wt(int_part);\n        \n        double frac_part = x - int_part;\n        if (frac_part > 0) {\n            wc(\'.\');\n            for (int i = 0; i < 10; ++i) {\n                frac_part *= 10;\n                int digit = static_cast<int>(frac_part);\n                wc(\'0\' + digit);\n                frac_part -= digit;\n                if (frac_part < 1e-10) break;\n            }\n        }\n        wt(sep);\n    }\n\n    template <typename T> inline void wt_ln(T x, string sep = \"\\n\") {\n\n        if (x == 0) {\n            wc(\'0\');\n            wt(sep);\n            return;\n        }\n        if (x < 0) {\n            wc(\'-\');\n            x = -x;\n        }\n        char *t = tmp_s;\n        while (x) {\n            T y = x / 10;\n            *(t++) = (x - y * 10) + \'0\';\n            x = y;\n        }\n        while (t != tmp_s) wc(*(--t));\n        wt(sep);\n    }\n\n  \n    inline void wt_ln(const char *s, string sep = \"\\n\") {\n            while (*s) {\n                wc(*(s++));\n            }\n        wt(sep);\n    }\n\n    inline void wt_ln(const char &c, string sep = \"\\n\") {\n        wc(c);\n        wt(sep);\n    }\n    inline void wt_ln(const string &s, string sep = \"\\n\") {\n        for(char c : s) wc(c);\n        wt(sep);\n    }\n\n    inline void wt(double x, int precision = 10, string sep = \"\\n\") {\n        if (x < 0) {\n            wc(\'-\');\n            x = -x;\n        }\n\n        long long head = static_cast<long long>(x);\n        wt(head);\n        \n        double tail = x - head;\n        if (tail > 0) {\n            wc(\'.\');\n            for (int i = 0; i < precision; ++i) {\n                tail *= 10;\n                int digit = static_cast<int>(tail);\n                wc(\'0\' + digit);\n                tail -= digit;\n                if (tail < 1e-10) break;\n            }\n        }\n    }\n\n    template <typename T> inline void wt(T x) {\n        if (x == 0) {\n            wc(\'0\');\n            return;\n        }\n        if (x < 0) {\n            wc(\'-\');\n            x = -x;\n        }\n        char *t = tmp_s;\n        while (x) {\n            T y = x / 10;\n            *(t++) = (x - y * 10) + \'0\';\n            x = y;\n        }\n        while (t != tmp_s) wc(*(--t));\n    }\n\n    inline void wt(const char *s)\n    {\n        while(*s) wc(*(s++));\n    }\n\n    inline void wt(const string &s) {\n        for (char c : s) {\n            wc(c);\n        }\n    }\n\n    inline void wt(const char &c) { wc(c); }\n\n    template <typename f, typename s> inline void read(pair<f, s> &PAIR) {\n        read(PAIR.first);\n        read(PAIR.second);\n    }\n    template <typename f, typename s>\n    inline void wt_ln(pair<f, s> &PAIR, string sep = \" \") {\n        wt(PAIR.first, sep);\n        wt(PAIR.second, sep);\n        wc(\'\\n\');\n    }\n    template <typename f, typename s>\n    inline void wt(pair<f, s> &PAIR, string sep = \" \", string end = \"\")\n    {\n        wt(PAIR.first, sep);\n        wt(PAIR.second, sep);\n        wt(end);\n    }\n\n    template <typename T> inline void read(vector<T> &vec) {\n        for (auto &v : vec) {\n            read(v);\n        }\n    }\n    template <typename T> inline void wt_ln(vector<T> &vec, string sep = \" \") {\n        int vec_size = vec.size();\n        for(int i = 0; i < vec_size; i++) {\n            if(i == vec_size - 1) wt_ln(vec[i], \"\");\n            else wt_ln(vec[i], sep);\n        }\n        wc(\'\\n\');\n    }\n    template <typename T> inline void wt(vector<T> &vec, string sep = \" \", string end = \"\") {\n        int vec_size = vec.size();\n        for(int i = 0; i < vec_size; i++) {\n            if(i == vec_size - 1) wt_ln(vec[i], \"\");\n            else wt_ln(vec[i], sep);\n        }\n        wt(end);\n    }\n\n    #undef wc\n    };\nFastIO io;\n#define wtln io.wt_ln\n#define wt io.wt\n#define rd io.read\n";
        std::string pos = HEAD;
    
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "FastIO_flush";
        std::string func_code = "class FastIO {\n    char tmp_s[20];\n\npublic:\n    FastIO() {}\n\n    ~FastIO() { flush(); }\n\n    inline void flush() {\n        fflush(stdout);\n    }\n\n    inline void wc(char x) {\n        putchar(x);\n        if (x == '\\n') flush();\n    }\n\n    inline void read(double &x) {\n        bool neg = false;\n        x = 0;\n        double factor = 1.0;\n        \n        char ch = getchar();\n        while ((ch < '0' || ch > '9') && ch != '-' && ch != '.')\n            ch = getchar();\n        \n        if (ch == '-') {\n            neg = true;\n            ch = getchar();\n        }\n\n        while ('0' <= ch && ch <= '9') {\n            x = x * 10 + (ch - '0');\n            ch = getchar();\n        }\n\n        if (ch == '.') {\n            ch = getchar();\n            while ('0' <= ch && ch <= '9') {\n                factor *= 0.1;\n                x += (ch - '0') * factor;\n                ch = getchar();\n            }\n        }\n\n        if (neg)\n            x = -x;\n    }\n\n    template <typename T> inline void read(T &x) {\n        bool neg = false;\n        x = 0;\n        char ch = getchar();\n        while ((ch < '0' || ch > '9') && ch != '-')\n            ch = getchar();\n        if (ch == '-') {\n            neg = true;\n            ch = getchar();\n        }\n        while ('0' <= ch && ch <= '9') {\n            x = 10 * x + (ch - '0');\n            ch = getchar();\n        }\n        if (neg)\n            x = -x;\n    }\n\n    inline void read(string &s) {\n        s.clear();\n        char ch = getchar();\n        while (ch <= ' ')\n            ch = getchar();\n        while (ch > ' ') {\n            s += ch;\n            ch = getchar();\n        }\n    }\n\n    inline void read(char &c) {\n        c = getchar();\n    }\n\n    inline void wt_ln(double x, char sep = '\\n') {\n        if (x < 0) {\n            wc('-');\n            x = -x;\n        }\n\n        long long int_part = static_cast<long long>(x);\n        wt(int_part);\n        \n        double frac_part = x - int_part;\n        if (frac_part > 0) {\n            wc('.');\n            for (int i = 0; i < 10; ++i) {\n                frac_part *= 10;\n                int digit = static_cast<int>(frac_part);\n                wc('0' + digit);\n                frac_part -= digit;\n                if (frac_part < 1e-10) break;\n            }\n        }\n        wc(sep);\n    }\n\n    template <typename T> inline void wt_ln(T x, char sep = '\\n') {\n        if (x == 0) {\n            wc('0');\n            wc(sep);\n            return;\n        }\n        if (x < 0) {\n            wc('-');\n            x = -x;\n        }\n        char *t = tmp_s;\n        while (x) {\n            T y = x / 10;\n            *(t++) = (x - y * 10) + '0';\n            x = y;\n        }\n        while (t != tmp_s)\n            wc(*(--t));\n        wc(sep);\n    }\n\n    inline void wt_ln(const char *s, char sep = '\\n') {\n        while (*s) {\n            wc(*(s++));\n        }\n        wc(sep);\n    }\n\n    inline void wt_ln(const char &c, char sep = '\\n') {\n        wc(c);\n        wc(sep);\n    }\n\n    inline void wt_ln(const string &s, char sep = '\\n') {\n        for (char c : s) wc(c);\n        wc(sep);\n    }\n\n    inline void wt(double x, int precision = 10) {\n        if (x < 0) {\n            wc('-');\n            x = -x;\n        }\n\n        long long head = static_cast<long long>(x);\n        wt(head);\n        \n        double tail = x - head;\n        if (tail > 0) {\n            wc('.');\n            for (int i = 0; i < precision; ++i) {\n                tail *= 10;\n                int digit = static_cast<int>(tail);\n                wc('0' + digit);\n                tail -= digit;\n                if (tail < 1e-10) break;\n            }\n        }\n    }\n\n    template <typename T> inline void wt(T x) {\n        if (x == 0) {\n            wc('0');\n            return;\n        }\n        if (x < 0) {\n            wc('-');\n            x = -x;\n        }\n        char *t = tmp_s;\n        while (x) {\n            T y = x / 10;\n            *(t++) = (x - y * 10) + '0';\n            x = y;\n        }\n        while (t != tmp_s)\n            wc(*(--t));\n    }\n\n    inline void wt(const char *s) {\n        while (*s) wc(*(s++));\n    }\n\n    inline void wt(const std::string &s) {\n        for (char c : s) {\n            wc(c);\n        }\n    }\n\n    inline void wt(const char &c) { wc(c); }\n\n    // pair<T1,T2>\n    template <typename f, typename s> inline void read(pair<f, s> &P) {\n        read(P.first);\n        read(P.second);\n    }\n    template <typename f, typename s>\n    inline void wt_ln(pair<f, s> &P, char sep = ' ') {\n        wt_ln(P.first, sep);\n        wt_ln(P.second, sep);\n        wc('\\n');\n    }\n\n    template <typename f, typename s>\n    inline void wt(pair<f, s> &P, char sep = ' ') {\n        wt(P.first);\n        wc(sep);\n        wt(P.second);\n    }\n\n    // vector<T>\n    template <typename T> inline void read(vector<T> &vec) {\n        for (auto &v : vec) {\n            read(v);\n        }\n    }\n    template <typename T> inline void wt_ln(vector<T> &vec, char sep = ' ') {\n        for (auto &v : vec) {\n            wt(v);\n            wc(sep);\n        }\n        wc('\\n');\n    }\n\n    template <typename T> inline void wt(vector<T> &vec, char sep = ' ') {\n        for (auto &v : vec) {\n            wt(v);\n            wc(sep);\n        }\n    }\n};\n\nFastIO io;\n#define wtln io.wt_ln\n#define wt io.wt\n#define rd io.read\n#define flush io.flush\n";
        std::string pos = HEAD;

        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "istream";
        std::string func_code = "";
        std::string pos = HEAD;

        std::vector<std::string> deps = {"istream_pair", "istream_vector"};
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "istream_pair";
        std::string func_code = "template <class T, class U>\nistream &operator>>(istream &is, pair<T, U> &p) {\n    is >> p.first >> p.second;\n    return is;\n}\n";
        std::string pos = HEAD;
        
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "istream_vector";
        std::string func_code = "template <class T>\nistream &operator>>(istream &is, vector<T> &v) {\n    fore(x, v) is >> x;\n    return is;\n}\n";
        std::string pos = HEAD;

        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "ostream";
        std::string func_code = "";
        std::string pos = HEAD;

        std::vector<std::string> deps = {"ostream_pair", "ostream_set", "ostream_map", "ostream_vector", "ostream_vector_vector"};
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "ostream_pair";
        std::string func_code = "template <class T, class U>\nostream &operator<<(ostream &os, const pair<T, U> &p) {\n    os << p.first << \" \" << p.second;\n    return os;\n}\n";
        std::string pos = HEAD;

        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "ostream_set";
        std::string func_code = "template <class T>\nostream &operator<<(ostream &os, set<T> st) {\n    fore(x, st) {\n        os << x << \" \";\n    }\n    return os;\n}\n";
        std::string pos = HEAD;

        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "ostream_map";
        std::string func_code = "template <class T1, class T2>\nostream &operator<<(ostream &os, map<T1, T2> mp) {\n    fore(x, mp) {\n        os <<  \" | \" << x.first << \"->\" << x.second << \" | \";\n    }\n    return os;\n}\n";
        std::string pos = HEAD;
        
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "ostream_vector";
        std::string func_code = "template <class T>\nostream &operator<<(ostream &os, const vector<T> &v) {\n    fore(x, v) os << x << ' ';\n    return os;\n}\n";
        std::string pos = HEAD;
        
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "ostream_vector_vector";
        std::string func_code = "template <class T>\nostream &operator<<(ostream &os, const vector<vector<T>> &v) {\n    fore(vv, v) {\n        fore(x, vv) os << x << ' ';\n        os << '\\n';\n    }\n    return os;\n}\n";
        std::string pos = HEAD;

        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "SYNC_WITH_STDIO";
        std::string func_code = "ios::sync_with_stdio(false);";
        std::string pos = MAIN;
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "CIN_TIE";
        std::string func_code = "cin.tie(nullptr);";
        std::string pos = MAIN;
        std::vector<std::string> deps;
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "chmax";
        std::string func_code = "template<class S, class T> bool chmax(S &a, T b){if(a<b) { a=b;return 1;} return 0;}";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "chmin";
        std::string func_code = "template<class S, class T> bool chmin(S &a, T b){if(a>b) { a=b;return 1;} return 0;}";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "biseek";
        std::string func_code = "template <typename T, typename S, typename Func> T biseek(T ok, S ng, Func f) {\n  while (abs(ok - ng) > 1) {\n    T mid = ok + (ng - ok) / 2;\n    if (f(mid))\n      ok = mid;\n    else\n      ng = mid;\n  }\n  return ok;\n}";
        std::string pos = HEAD;
        std::vector<std::string> deps;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_dependencies[func_name] = deps;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "runLengthEncoding";
        std::string func_code = "vector<pair<char, int>> runLengthEncoding(string s){\n    int n = s.length();\n    vector<pair<char, int>> res;\n    char pre = s[0];\n    int cnt = 1;\n    for(int i = 1;i < n; i++){\n        if (pre != s[i]){\n            res.push_back({ pre, cnt });\n            pre = s[i];\n            cnt = 1;\n        }\n        else{\n            cnt++;\n        }\n    }\n    res.push_back({ pre, cnt });\n    return res;\n}\n";
        std::string pos = HEAD;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "CycleLib";
        std::string func_code = "template <class T>\nstruct CycleLib {\n    T n;\n    CycleLib(T n) : n(n) {}\n    CycleLib() : n(0) {}\n\n    \n    T move(T s, T i) {\n        return _mod(s + i, n);\n    }\n    T rmove(T s, T i) {\n        return _mod(s - i, n);\n    }\n\n    \n    T dist(T s, T t) {\n        return _mod(t - s, n);\n    }\n    T rdist(T s, T t) {\n        return _mod(s - t, n);\n    }\n\n    \n    T dist_min(T s, T t) {\n        T d = _mod(t - s, n);\n        chmin(d, n - d);\n        return d;\n    }\n\n   \n    bool is_crs_m(T s, T m, T t) {\n        return dist(s, m) <= dist(s, t);\n    }\n    bool ris_crs_m(T s, T m, T t) {\n        return rdist(s, m) <= rdist(s, t);\n    }\n};\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        
        deps.push_back("_mod");
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
        lib_dependencies[func_name] = deps;
    }
    {
        std::string func_name = "yesno";
        std::string func_code = "string yesno(int ok, string yes = \"Yes\", string no = \"No\") {\n  return ok ? yes : no;\n}\n";
        std::string pos = HEAD;
    
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
    }
    {
        std::string func_name = "inversionnumber";
        std::string func_code = "template<class T> long long inversionNumber(vector<T> &x, vector<T> &y){\n    int sz = x.size();\n    assert(sz == (int)y.size());\n    Fenwick_tree<long long> bit(sz);\n    map<T, vector<int>> idx;\n    for(int i=sz-1;i>=0;i--){\n        idx[x[i]].emplace_back(i);\n    }\n    long long res = 0;\n    for(int i=0;i<sz;i++){\n        vector<int> &v = idx[y[i]];\n        if(v.empty()){\n            return -1;\n        }\n        int id = v.back();\n        v.pop_back();\n        res += id - bit.sum(id);\n        bit.add(id, 1);\n    }\n    return res;\n}\n";
        std::string pos = HEAD;
        std::vector<std::string> deps;
        deps.push_back("Fenwick_tree");
        lib_names.push_back(func_name);
        lib_Ident[func_name] = func_code;
        lib_positions[func_name] = pos;
        lib_dependencies[func_name] = deps;
    }
}

void LibraryManager::init() {
    initBasicSettings();
    initMathLibraries();
    initDataStructures();
    initUtilityFunctions();

    if (PRAGMA)
        insert_is_Still.insert("pragma");
    if (STDCPP)
        insert_is_Still.insert("stdc++");
    if (NAMESPACE_STD)
    {
        insert_is_Still.insert("namespace_std");
    }
}