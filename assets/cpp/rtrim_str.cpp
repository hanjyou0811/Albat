pair<string, int> rtrim(const string& s, char ch) {
    int cnt = 0;
    int last = (int)s.size() - 1;
    while (last >= 0 && s[last] == ch) {
        last--;
        cnt++;
    }
    if (last < 0) return {"", cnt};
    return {s.substr(0, last + 1), cnt};
}
