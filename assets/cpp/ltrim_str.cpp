pair<string, int> ltrim(const string& s, char ch) {
    int cnt = 0;
    while (cnt < (int)s.size() && s[cnt] == ch) {
        cnt++;
    }
    return {s.substr(cnt), cnt};
}
