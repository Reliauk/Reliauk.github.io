#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 5E5 + 5;

int n, m;
std::vector<int> raw;
std::array<int, N> a, b;

struct I {
  int l, r;
  i64 mn, mx, cv, cd, v;
};
std::array<I, N * 8> t;
#define LC (o + o)
#define RC (o + o + 1)
void Up(int o) {
  t[o].mn = std::min(t[LC].mn, t[RC].mn);
  t[o].mx = std::max(t[LC].mx, t[RC].mx);
}
void Cov(int o, i64 v, i64 d) {
  t[o].mn = v + d * raw[t[o].l], t[o].mx = v + d * raw[t[o].r];
  t[o].cv = v, t[o].cd = d, t[o].v = 0;
}
void Upd(int o, i64 v) { t[o].v += v, t[o].mn += v, t[o].mx += v; }
void Push(int o) {
  if (~t[o].cd) {
    Cov(LC, t[o].cv, t[o].cd), Cov(RC, t[o].cv, t[o].cd);
    t[o].cv = t[o].cd = -1;
  }
  if (t[o].v) Upd(LC, t[o].v), Upd(RC, t[o].v), t[o].v = 0;
}
void Make(int o, int l, int r) {
  t[o].l = l, t[o].r = r;
  t[o].cv = t[o].cd = -1, t[o].v = t[o].mn = t[o].mx = 0;
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
void Cover(int o, int x, int y, i64 v, i64 d) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return Cov(o, v, d);
  if (x > r || l > y) return;
  Push(o), Cover(LC, x, y, v, d), Cover(RC, x, y, v, d), Up(o);
}
i64 Query(int o, int x) {
  int l = t[o].l, r = t[o].r, m = (l + r) >> 1;
  if (l == r) return t[o].mn;
  return Push(o), Query(LC + (x > m), x);
}
std::vector<int> seg;
void Loc(int o, int x, int y) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return void(seg.push_back(o));
  if (x > r || l > y) return;
  Push(o), Loc(LC, x, y), Loc(RC, x, y);
}
int FirstG(int l, int r, i64 v, i64 d) {
  seg.clear(), Loc(1, l, r);
  for (int o : seg) {
    if (t[o].mx <= v + d * raw[t[o].r]) continue;
    while (t[o].l - t[o].r) {
      int m = (t[o].l + t[o].r) >> 1;
      Push(o), o = LC + (t[LC].mx <= v + d * raw[m]);
    }
    return t[o].l;
  }
  return r + 1;
}
int FirstL(int l, int r, i64 v, i64 d) {
  seg.clear(), Loc(1, l, r);
  for (int o : seg) {
    if (t[o].mx >= v + d * raw[t[o].r]) continue;
    while (t[o].l - t[o].r) {
      int m = (t[o].l + t[o].r) >> 1;
      Push(o), o = LC + (t[LC].mx >= v + d * raw[m]);
    }
    return t[o].l;
  }
  return r + 1;
}
#undef LC
#undef RC

void Proc() {
  std::cin >> n >> m, raw = {0};
  for (int i = 1; i <= n; ++i) {
    std::cin >> a[i] >> b[i], b[i] *= -1;
    raw.push_back(a[i]), raw.push_back(a[i] - 1);
  }
  sort(ALL(raw)), raw.erase(unique(ALL(raw)), end(raw));

  Make(1, 0, raw.size() - 1), Cover(1, 0, raw.size() - 1, 0, m);
  for (int i = 1; i <= n; ++i) {
    int it = std::lower_bound(ALL(raw), a[i]) - begin(raw);
    Add(1, it, raw.size() - 1, b[i]);
    i64 val = Query(1, it);
    int p = FirstG(0, it - 1, val, 0);
    if (p < it) Cover(1, p, it - 1, val, 0);
    val = Query(1, it - 1) + i64(1 - a[i]) * m;
    p = FirstL(it, raw.size() - 1, val, m);
    if (it < p) Cover(1, it, p - 1, val, m);
  }

  std::cout << -t[1].mn << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}