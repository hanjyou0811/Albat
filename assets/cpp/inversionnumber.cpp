template<class T> long long inversionNumber(vector<T> &x, vector<T> &y){
    int sz = x.size();
    assert(sz == (int)y.size());
    Fenwick_tree<long long> bit(sz);
    map<T, vector<int>> idx;
    for(int i=sz-1;i>=0;i--){
        idx[x[i]].emplace_back(i);
    }
    long long res = 0;
    for(int i=0;i<sz;i++){
        vector<int> &v = idx[y[i]];
        if(v.empty()){
            return -1;
        }
        int id = v.back();
        v.pop_back();
        res += id - bit.sum(id);
        bit.add(id, 1);
    }
    return res;
}
