#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 5E5 + 5, INF = 1E9;

int n;
std::array<i64, N> a;

std::array<int, N> od, f;
struct F {
  std::array<int, N> bit;
  void Reset() { std::fill(PAR(bit, 0, n + 2), 0); }
  void Add(int x, int v) {
    for (++x; x <= n + 1; x += x & -x) bit[x] += v;
  }
  int Query(int x) {
    int v = 0;
    for (++x; x; x &= x - 1) v += bit[x];
    return v;
  }
} lt, gt;

struct I {
  int l, r, v, g, s;
  int w, c, wg;
};
std::array<I, N * 4> t;
#define LC (o + o)
#define RC (o + o + 1)
void Up(int o) {
  t[o].v = std::min(t[LC].v, t[RC].v);
  t[o].g = std::min(t[LC].g, t[RC].g);
  t[o].s = std::min(t[LC].s, t[RC].s);
}
void UpdV(int o, int v) { t[o].v += v, t[o].s += v, t[o].w += v; }
void UpdVG(int o, int v) { t[o].g += v, t[o].s += v, t[o].wg += v; }
void UpdG(int o, int c) {
  t[o].g = c, t[o].s = c + t[o].v, t[o].c = c, t[o].wg = 0;
}
void Push(int o) {
  if (t[o].w) UpdV(LC, t[o].w), UpdV(RC, t[o].w), t[o].w = 0;
  if (t[o].c < INF) UpdG(LC, t[o].c), UpdG(RC, t[o].c), t[o].c = INF;
  if (t[o].wg) UpdVG(LC, t[o].wg), UpdVG(RC, t[o].wg), t[o].wg = 0;
}
void Make(int o, int l, int r) {
  t[o].l = l, t[o].r = r, t[o].w = t[o].wg = 0, t[o].c = INF;
  if (l == r)
    return t[o].v = -l - 1, t[o].g = INF, void(t[o].s = t[o].g + t[o].v);
  int m = (l + r) >> 1;
  Make(LC, l, m), Make(RC, m + 1, r), Up(o);
}
void Add(int o, int x, int y, int v) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return UpdV(o, v);
  if (x > r || l > y) return;
  Push(o), Add(LC, x, y, v), Add(RC, x, y, v), Up(o);
}
void AddG(int o, int x, int y, int v) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return UpdVG(o, v);
  if (x > r || l > y) return;
  Push(o), AddG(LC, x, y, v), AddG(RC, x, y, v), Up(o);
}
int Ans(int o, int x, int y) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return t[o].s;
  if (x > r || l > y) return INF;
  return Push(o), std::min(Ans(LC, x, y), Ans(RC, x, y));
}
int First(int o, int v) {
  if (t[o].g >= v) return t[o].r + 1;
  if (t[o].l == t[o].r) return t[o].l;
  return Push(o), First(LC + (t[LC].g >= v), v);
}
void Cover(int o, int x, int y, int v) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return UpdG(o, v);
  if (x > r || l > y) return;
  Push(o), Cover(LC, x, y, v), Cover(RC, x, y, v), Up(o);
}
void Chkmin(int p, int v) {
  int q = First(1, v);
  if (p < q) Cover(1, p, q - 1, v);
}
#undef LC
#undef RC

void Proc() {
  std::cin >> n;
  for (int i = 1; i <= n; ++i) std::cin >> a[i], a[i] += a[i - 1];
  if (a[n] < 0) return void(std::cout << "No\n");
  std::cout << "Yes\n";

  for (int i = 0; i <= n; ++i) od[i] = i;
  std::sort(PAR(od, 0, n + 1), [&](int u, int v) { return a[u] < a[v]; });

  lt.Reset(), gt.Reset(), Make(1, 0, n);
  for (int i = 0; i <= n; ++i) gt.Add(i, 1), f[i] = INF;
  for (int i = f[0] = 0, p = 0, q = 0; i <= n; ++i) {
    for (; p <= n && a[od[p]] <= a[od[i]]; ++p)
      gt.Add(od[p], -1), Add(1, od[p], n, 1);
    for (; q <= n && a[od[q]] < a[od[i]]; ++q)
      lt.Add(od[q], 1), Add(1, od[q], n, 1), AddG(1, od[q], n, -1);
    if (od[i]) f[od[i]] = Ans(1, 0, od[i] - 1) + gt.Query(od[i]);
    Chkmin(od[i], f[od[i]] - lt.Query(od[i]) - 1);
  }

  std::cout << f[n] + n << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}