string Slice(string &str, int st, int ed, int step) {
    int siz = str.size();
    int i,j,k;
    string res;
    if(siz == 0) return res;
    k = step %+ siz;
    if(step > 0) {
        for(i = st, j = (st %+ siz); i < ed; i += step, j += k) {
            if(j >= siz) j -= siz;
            res += str[j];
        }
    }else{
        for(i = st, j = (st %+ siz); i > ed; i += step, j += k) {
            if(j >= siz) j -= siz;
            res += str[j];
        }
    }
    return res;
}
