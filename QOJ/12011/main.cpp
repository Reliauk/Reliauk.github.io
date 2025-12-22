#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using i128 = __int128;

const int X = 1E6 + 45;
const i64 INF = 1E18;

int q;
std::array<int, X> a;

int R(int v, int k) { return k > 30 ? 0 : v >> k; }
i64 L(i64 v, int k) {
  if (!v) return 0;
  return k <= 60 ? std::min<i128>(INF, i128(v) << k) : INF;
}
struct S {
  int v, n;
  i64 s0, s1, a;
  S() { s0 = s1 = v = a = n = 0; }
  S(int v_, int n_, i64 s0_, i64 s1_, i64 a_) {
    v = v_, n = n_, s0 = s0_, s1 = s1_, a = a_;
  }
  i64 operator()() { return s0 - v; }
  S operator+(const S& o) const {
    if (!n) return o;
    if (!o.n) return *this;
    S r;
    r.s0 = s0 + o.s0, r.s1 = s1 + o.s1, r.n = n + o.n;
    i64 aa = o.a - v % L(1, o.n);
    if (aa <= 0) aa += L(1, o.n);
    r.a = std::min(INF, L(aa - 1, n) + a);
    r.v = o.v + R(v, o.n) + (v % L(1, o.n) >= o.a);
    return r;
  }
};
std::array<S, X> s;
struct I {
  int l, r;
  S s;
};
std::array<I, X * 4> t;
#define LC (o + o)
#define RC (o + o + 1)
void Leaf(int o, int x) {
  t[o].s = s[x] = S(a[x] / 2, 1, a[x], i64(a[x]) * x, 2 - a[x] % 2);
}
void Up(int o) { t[o].s = t[LC].s + t[RC].s; }
void Make(int o, int l, int r) {
  t[o].l = l, t[o].r = r;
  if (l == r) return Leaf(o, l);
  int m = (l + r) >> 1;
  Make(LC, l, m), Make(RC, m + 1, r), Up(o);
}
void Upd(int o, int x) {
  if (x == 1) return;
  int l = t[o].l, r = t[o].r, m = (l + r) >> 1;
  if (l == r) return Leaf(o, x);
  Upd(LC + (x > m), x), Up(o);
}
std::pair<S, int> Find(i64 k) {
  S pre;
  int o = 1, l = 2, r = 1E6 + 42;
  while (l - r) {
    int m = (l + r) >> 1;
    if ((pre + t[LC].s)() <= k)
      pre = pre + t[LC].s, l = m + 1, o = RC;
    else
      r = m, o = LC;
  }
  return {pre, l};
}
#undef LC
#undef RC

i64 Ans(i64 k) {
  i64 kk = k;
  if (k -= a[1]; k <= 0) return k + a[1];
  i64 ans = a[1];
  auto [pre, x] = Find(k);
  ans += pre.s1 - i64(pre.v) * x, k -= pre(), ans += k * i64(x - 1);
  if (k > (pre.v + a[x]) / 2) ++ans;
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> q, Make(1, 2, 1E6 + 42);
  for (int op; q; --q) {
    i64 x;
    if (std::cin >> op >> x; op == 1)
      std::cin >> a[x], Upd(1, x);
    else
      std::cout << Ans(x) << '\n';
  }

  return 0;
}