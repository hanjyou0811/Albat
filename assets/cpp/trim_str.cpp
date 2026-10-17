pair<string,int> trim(const string& s, char ch) {
    auto[ltrimed, lcnt] = ltrim(s, ch);
    auto [rtrimed, rcnt] = rtrim(ltrimed, ch);
    return {rtrimed, lcnt+rcnt};
}
