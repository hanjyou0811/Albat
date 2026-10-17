template<typename T>
pair<vector<T>, int> rtrim(const vector<T>& v, T ch) {
    int cnt = 0;
    int last = (int)v.size() - 1;
    while (last >= 0 && v[last] == ch) {
        last--;
        cnt++;
    }
    if (last < 0) return {{}, cnt};
    return {
        vector<T>(v.begin(), v.begin() + last + 1),
        cnt
    };
}
