template<class T, class S> inline T _pow(T a, S b){
  T res = 1;
  res = 1;
  for(;;){
    if(b&1) res *= a;
    b >>= 1;
    if(b==0) break;
    a *= a;
  }
  return res;
}
inline double _pow(double a, double b){
  return pow(a,b);
}
