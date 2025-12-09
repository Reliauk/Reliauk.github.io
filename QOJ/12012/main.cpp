#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5, Q = 5E5 + 5;

int n, q;
std::array<int, Q> ql, qr, ans;
std::array<int, N> rx, ry, rw, bx, by, bw;

std::vector<int> raw;
std::array<int, N * 2> yr, yb;

std::vector<int> lp, rp;
std::vector<std::pair<int, int>> dom;
void Proc(int l, int r) {
  if (l + 1 == r) return;
  int m = (l + r) >> 1;
  Proc(l, m), Proc(m, r);

  lp.clear(), rp.clear();
  for (int i = l; i < m; ++i)
    if (yr[i]) lp.push_back(yr[i]);
  for (int i = m; i < r; ++i)
    if (yb[i]) rp.push_back(yb[i]);
  if (lp.empty() || rp.empty()) return;

  int lx = *max_element(ALL(lp), [&](int u, int v) { return rw[u] < rw[v]; });
  int rx = *max_element(ALL(rp), [&](int u, int v) { return bw[u] < bw[v]; });
  for (int i : rp) dom.emplace_back(lx, i);
  for (int i : lp) dom.emplace_back(i, rx);
}

std::array<int, Q> od;
std::array<int, N + Q> bit;
void Reset() { bit.fill(-1); }
void Add(int x, int v) {
  for (; x <= raw.size(); x += x & -x) bit[x] = std::max(bit[x], v);
}
int Query(int x) {
  int v = -1;
  for (; x; x &= x - 1) v = std::max(v, bit[x]);
  return v;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  for (int i = 1; i <= n; ++i) std::cin >> rx[i] >> ry[i] >> rw[i];
  for (int i = 1; i <= n; ++i) std::cin >> bx[i] >> by[i] >> bw[i];
  std::cin >> q, ans.fill(-1);
  for (int i = 1; i <= q; ++i) std::cin >> ql[i] >> qr[i];

  raw = std::vector(PAR(ry, 1, n + 1)), raw.insert(end(raw), PAR(by, 1, n + 1));
  auto zip = [&](int x) { return std::lower_bound(ALL(raw), x) - begin(raw); };
  sort(ALL(raw));
  for (int i = 1; i <= n; ++i) yr[zip(ry[i])] = yb[zip(by[i])] = i;

  Proc(0, raw.size());

  for (int i = 1; i <= q; ++i) od[i] = i;
  std::sort(PAR(od, 1, q + 1), [&](int u, int v) { return ql[u] < ql[v]; });
  sort(ALL(dom), [&](auto u, auto v) { return rx[u.first] < rx[v.first]; });
  raw = std::vector(PAR(bx, 1, n + 1)), raw.insert(end(raw), PAR(qr, 1, q + 1));
  sort(ALL(raw)), Reset();
  for (int o = 1, i = od[1], ptr = 0; o <= q; i = od[++o]) {
    while (ptr < dom.size() && rx[dom[ptr].first] < ql[i]) {
      auto [x, y] = dom[ptr++];
      Add(raw.size() - zip(bx[y]), rw[x] + bw[y]);
    }
    ans[i] = std::max(ans[i], Query(raw.size() - zip(qr[i]) - 1));
  }
  reverse(ALL(dom)), Reset();
  for (int o = q, i = od[q], ptr = 0; o; i = od[--o]) {
    while (ptr < dom.size() && rx[dom[ptr].first] > ql[i]) {
      auto [x, y] = dom[ptr++];
      Add(zip(bx[y]) + 1, rw[x] + bw[y]);
    }
    ans[i] = std::max(ans[i], Query(zip(qr[i])));
  }

  for (int i = 1; i <= q; ++i) std::cout << ans[i] << '\n';

  return 0;
}