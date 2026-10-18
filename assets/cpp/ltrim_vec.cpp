template<typename T>
pair<vector<T>, int> ltrim(const vector<T>& v, T ch) {
    int cnt = 0;
    while (cnt < (int)v.size() && v[cnt] == ch) {
        cnt++;
    }
    return {
        vector<T>(v.begin() + cnt, v.end()),
        cnt
    };
}
