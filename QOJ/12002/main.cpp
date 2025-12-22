#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5;

i64 s;
int n, q;
std::array<i64, N> a, ans;
std::array<int, N> ql, qr;

std::array<int, N> rad;
std::array<i64, N> s0, s1;
i64 S0(int l, int r) { return s0[r] - s0[l - 1]; }
i64 S1(int l, int r) { return s1[r] - s1[l - 1]; }
i64 F(int l, int x) {
  if (s <= a[x]) return 0;
  int y = std::min(rad[x], x - l);
  i64 ans = (y + 1) * (s - a[x]);
  ans -= S1(x - y, x - 1) + S0(x - y, x - 1) * (y + 1 - x);
  ans -= S0(x + 1, x + y) * (x + y + 1) - S1(x + 1, x + y);
  return ans;
}

struct I {
  int l, r;
  std::vector<std::pair<int, int>> eva;
};
std::array<I, N * 4> t;
#define LC (o + o)
#define RC (o + o + 1)
void Make(int o, int l, int r) {
  t[o].l = l, t[o].r = r, t[o].eva.clear();
  if (l == r) return;
  int m = (l + r) >> 1;
  Make(LC, l, m), Make(RC, m + 1, r);
}
void Hang(int o, int x, int y, int z, int i) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return void(t[o].eva.emplace_back(z, i));
  if (x > r || l > y) return;
  Hang(LC, x, y, z, i), Hang(RC, x, y, z, i);
}
std::vector<std::pair<int, int>> p;
void Rec(int l, int r, int x, int y) {
  if (l > r) return;
  int m = (l + r) >> 1, q = x;
  for (int i = x; i <= y; ++i)
    if (F(p[m].first, i) > F(p[m].first, q)) q = i;
  ans[p[m].second] = std::max(ans[p[m].second], F(p[m].first, q));
  Rec(l, m - 1, x, q), Rec(m + 1, r, q, y);
}
void Proc(int o) {
  if (t[o].eva.size())
    p = t[o].eva, sort(ALL(p)), Rec(0, p.size() - 1, t[o].l, t[o].r);
  if (t[o].l - t[o].r) Proc(LC), Proc(RC);
}
#undef LC
#undef RC

void Proc() {
  for (int i = 1; i <= n; ++i)
    s0[i] = s0[i - 1] + a[i], s1[i] = s1[i - 1] + a[i] * i;
  for (int i = 1; i <= n; ++i) {
    int lo = 0, hi = std::min(i, n - i);
    while (lo < hi) {
      int mi = (lo + hi) >> 1;
      if (S0(i - mi, i + mi) >= s)
        hi = mi;
      else
        lo = mi + 1;
    }
    rad[i] = lo - 1;
  }
  Make(1, 1, n);
  for (int i = 1; i <= q; ++i) {
    int mid = (ql[i] + qr[i]) >> 1;
    Hang(1, ql[i], (ql[i] + qr[i]) >> 1, ql[i], i);
  }
  Proc(1);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> s;
  for (int i = 1; i <= n; ++i) std::cin >> a[i];
  std::cin >> q;
  for (int i = 1; i <= q; ++i) std::cin >> ql[i] >> qr[i];

  Proc(), std::reverse(PAR(a, 1, n + 1));
  for (int i = 1; i <= q; ++i)
    ql[i] = n + 1 - ql[i], qr[i] = n + 1 - qr[i], std::swap(ql[i], qr[i]);
  Proc();

  for (int i = 1; i <= q; ++i) std::cout << ans[i] << '\n';

  return 0;
}