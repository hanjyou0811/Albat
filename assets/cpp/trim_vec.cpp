template<typename T>
pair<vector<T>, int> trim(vector<T> s, T ch) {
    auto[ltrimed, lcnt] = ltrim(s, ch);
    auto [rtrimed, rcnt] = rtrim(ltrimed, ch);
    return {rtrimed, lcnt+rcnt};
}
