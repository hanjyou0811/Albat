template<class T, class S> inline T GCD_L(T a, S b){T r; while(b) r=a, a=b, b=r%a; return a;}
