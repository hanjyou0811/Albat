template<class S, class T> inline S _mod(S a, const T b){
  S x = a;
 x %= b;
  if(x < 0) x += b;
  return x;
}
