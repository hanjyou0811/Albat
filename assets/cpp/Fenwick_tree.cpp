template <class T> struct Fenwick_tree {
  int n;
  vector<T> bit;

  Fenwick_tree(int n_) : n(n_), bit(n + 1) {}

  void add(int pos, T x)
  {
    pos++;
while(pos <= n) {
      bit[pos] += x;
      pos += pos & -pos;
    }
  }

  T sum(int pos)
  {
    T res = 0;
    while(pos)
    {
      res += bit[pos];
      pos -= pos & -pos;
    }
    return res;
  }
  // [l, r)の合計
  T sum(int l, int r)
  {
    return sum(r) - sum(l);
  }
};