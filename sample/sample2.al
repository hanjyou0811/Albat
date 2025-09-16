{
    while(int@--) solve();
}
void solve(){
    int@n,k;
    vi@(p(n), q(n));
    int ok = 0;
    fore(pi, p) fore(qi, q) ok |= (pi + qi == k);
    $$(yesno(ok));
}