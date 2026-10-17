template <typename T, typename S, typename Func> T biseek(T ok, S ng, Func f) {
  while (abs(ok - ng) > 1) {
    T mid = ok + (ng - ok) / 2;
    if (f(mid))
      ok = mid;
    else
      ng = mid;
  }
  return ok;
}