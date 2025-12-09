#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 3E5 + 5;

int n, q, c;
std::array<i64, N> a;

struct S {
  i64 s, suf, s0, s1;
  S() = default;
  S(i64 s_, i64 suf_, i64 s0_, i64 s1_) {
    s = s_, suf = suf_, s0 = s0_, s1 = s1_;
  }
  S operator+(const S &o) const {
    return S(s + o.s, std::max(suf + o.s, o.suf), s0 + o.s0, s1 + o.s1);
  }
};
struct I {
  int l, r;
  S s;
};
std::array<I, N * 4> t;
#define LC (o + o)
#define RC (o + o + 1)
void Up(int o) { t[o].s = t[LC].s + t[RC].s; }
void Make(int o, int l, int r) {
  t[o].l = l, t[o].r = r;
  if (l == r)
    return void(t[o].s = S(a[l] / c - a[l] % c, a[l] / c, a[l], a[l] / c));
  int m = (l + r) >> 1;
  Make(LC, l, m), Make(RC, m + 1, r), Up(o);
}
void Upd(int o, int x) {
  int l = t[o].l, r = t[o].r, m = (l + r) >> 1;
  if (l == r)
    return void(t[o].s = S(a[l] / c - a[l] % c, a[l] / c, a[l], a[l] / c));
  Upd(LC + (x > m), x), Up(o);
}
#undef LC
#undef RC

i64 Ans() {
  int o = 1, l = 0, r = n;
  S pre(0, 0, 0, 0);
  while (l - r) {
    int m = (l + r) >> 1;
    S nxt = pre + t[o + o].s;
    if (nxt.suf + nxt.s0 + a[m + 1] + a[m + 1] / c >= t[1].s.s0)
      o += o, r = m;
    else
      pre = nxt, o += o + 1, l = m + 1;
  }
  pre = pre + t[o].s;
  i64 tic = pre.suf, lo = 0, hi = std::min(tic, a[++l]);
  while (lo < hi) {
    i64 mi = (lo + hi + 1) >> 1;
    if (tic + pre.s0 + a[l] - mi + (a[l] - mi) / c >= t[1].s.s0)
      lo = mi;
    else
      hi = mi - 1;
  }
  return pre.s0 + a[l] - lo + tic - pre.s1;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> q >> c;
  for (int i = 1; i <= n; ++i) std::cin >> a[i];

  Make(1, 0, n), std::cout << Ans() << '\n';
  for (int x; q; --q)
    std::cin >> x >> a[x], Upd(1, x), std::cout << Ans() << '\n';

  return 0;
}