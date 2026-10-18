template<class T> vector<T> Slice(vector<T> &vec, int st, int ed, int step) {
    int siz = vec.size();
    int i,j,k;
    vector<T> res;
    if(siz == 0) return res;
    k = step %+ siz;
    if(step > 0) {
        for(i = st, j = (st %+ siz); i < ed; i += step, j += k) {
            if(j >= siz) j -= siz;
            res.push_back(vec[j]);
        }
    }else{
        for(i = st, j = (st %+ siz); i > ed; i += step, j += k) {
            if(j >= siz) j -= siz;
            res.push_back(vec[j]);
        }
    }
    return res;
}
