template <class T> struct WeightedUnionFind {
  vector<int> par;
  vector<int> rank;
  vector<T> diff_weight;

  WeightedUnionFind(int n = 1, T sum = 0) { init(n, sum); }
  void init(int n = 1, T sum = 0) {
    par.resize(n);
    rank.resize(n);
    diff_weight.resize(n);
    for (int i = 0; i < n; i++)
      par[i] = i, rank[i] = 0, diff_weight[i] = sum;
  }

  int root(int x) {
    if (par[x] == x) {
      return x;
    } else {
      int r = root(par[x]);
      diff_weight[x] += diff_weight[par[x]];
      return par[x] = r;
    }
  }

  T weight(int x) {
    root(x);
    return diff_weight[x];
  }

  bool issame(int x, int y) { return root(x) == root(y); }

  bool merge(int x, int y, T w) {
    w += weight(x);
    w -= weight(y);
    x = root(x);
    y = root(y);
    if (x == y)
      return false;
    if (rank[x] < rank[y])
      swap(x, y), w = -w;
    if (rank[x] == rank[y])
      ++rank[x];
    par[y] = x;
    diff_weight[y] = w;
    return true;
  }

  T diff(int x, int y) { return weight(y) - weight(x); }
};
