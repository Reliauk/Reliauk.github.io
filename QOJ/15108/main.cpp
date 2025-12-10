#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using i128 = __int128;

const int N = 3E5 + 5;

int n, m;
std::vector<std::pair<int, i64>> ch;
std::array<i64, N> a, c, sa, sc, ans;

struct I {
  int l, r;
  i64 s0, s1, v;
  bool c;
};
std::array<I, N * 4> t;
#define LC (o + o)
#define RC (o + o + 1)
void Up(int o) {
  t[o].s0 = t[LC].s0 + t[RC].s0;
  t[o].s1 = t[LC].s1 + t[RC].s1;
}
void Upd(int o, i64 v) {
  t[o].v += v, t[o].s0 += (t[o].r - t[o].l + 1) * v;
  t[o].s1 += (sc[t[o].r] - sc[t[o].l - 1]) * v;
}
void Cov(int o) { t[o].s0 = t[o].s1 = t[o].v = 0, t[o].c = true; }
void Push(int o) {
  if (t[o].c) Cov(LC), Cov(RC), t[o].c = false;
  if (t[o].v) Upd(LC, t[o].v), Upd(RC, t[o].v), t[o].v = 0;
}
void Make(int o, int l, int r) {
  t[o].l = l, t[o].r = r;
  if (l == r) return;
  int m = (l + r) >> 1;
  Make(LC, l, m), Make(RC, m + 1, r);
}
void Add(int o, int x, int y, i64 v) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return Upd(o, v);
  if (x > r || l > y) return;
  Push(o), Add(LC, x, y, v), Add(RC, x, y, v), Up(o);
}
void Reset(int o, int x, int y) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return Cov(o);
  if (x > r || l > y) return;
  Push(o), Reset(LC, x, y), Reset(RC, x, y), Up(o);
}
i64 Sum(int o, int x, int y) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return t[o].s1;
  if (x > r || l > y) return 0;
  return Push(o), Sum(LC, x, y) + Sum(RC, x, y);
}
#undef LC
#undef RC

std::map<int, i64> vmo;
std::array<int, N> f;
std::array<i64, N> g;

void Modify(int p, int it) {
  i64 sum = sc[it] - a[p + 1] + Sum(1, 1, std::min(it, m));
  Reset(1, 1, std::min(it, m));
  while (vmo.size() && begin(vmo)->first <= it)
    sum += begin(vmo)->second, vmo.erase(begin(vmo));
  if (it <= m) {
    Add(1, it, it, sum / c[it]);
    if (sum % c[it]) vmo[it] = sum % c[it];
  }
  if (it < m) Add(1, it + 1, m, 1);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int i = 1; i <= n; ++i) std::cin >> a[i], sa[i] = sa[i - 1] + a[i];
  for (int i = 1; i <= m; ++i) std::cin >> c[i], sc[i] = sc[i - 1] + c[i];

  ch = {{n, sa[n]}}, Make(1, 1, m);
  for (int p = n - 1; ~p; --p) {
    for (; ch.size() >= 2; ch.pop_back()) {
      auto [a, b] = ch.back();
      auto [c, d] = end(ch)[-2];
      if (i128(p - a) * (d - b) < i128(c - a) * (sa[p] - b)) break;
    }
    i64 slop = (ch.back().second - sa[p]) / (ch.back().first - p);
    Modify(p, std::upper_bound(PAR(sc, 1, m + 1), slop) - data(sc));
    ch.emplace_back(p, sa[p]);
    i64 ans = t[1].s0, r = 0;
    for (auto [x, v] : vmo) ans += v > r, r += (v > r) * c[x] - v;
    ::ans[p] = i64(n - p) * m - ans;
  }

  for (int i = 0; i < n; ++i) std::cout << ans[i] << "\n "[i < n - 1];

  return 0;
}