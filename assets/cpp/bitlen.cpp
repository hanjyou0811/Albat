template<class T> inline int bitlen(T x){
    int len = 0;
    do {
        len++;
        x >>= 1;
    } while (x);
    return len;
}
