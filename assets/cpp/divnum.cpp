template<typename T>
vector<T> divNum(T n) { 
    vector<T> S;
    for (int i = 1; 1LL*i*i <= n; i++) if (n%i == 0) { S.push_back(i); if (i*i != n) S.push_back(n / i); }
    sort(S.begin(), S.end());
    return S;
}
