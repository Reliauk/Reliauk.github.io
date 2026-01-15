#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int M = 5E4 + 5, MOD = 998244353;
void Add(int &x, i64 y) { x = (x + y) % MOD; }
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}

i64 n;
int a, b, ans;
std::array<i64, M> u, v, w;
std::array<std::array<i64, 2>, M> adj;

std::array<i64, M> cc;
std::array<std::vector<i64>, M> pre;
std::vector<i64> nxt;
i64 Cnt(i64 n, std::vector<std::array<i64, 2>> v) {
  std::sort(ALL(v), std::greater());
  while (!v.back()[0]) v.pop_back();
  nxt.push_back(v.back()[1]);
  if (v.back()[0] >= n) return n;
  if (v.size() == 1) return v[0][0];
  i64 k = (n - 1) / v.back()[0];
  for (int i = 0; i + 1 < v.size(); ++i) k = std::min(k, v[i][0] / v.back()[0]);
  n -= k * v.back()[0];
  for (int i = 0; i + 1 < v.size(); ++i) v[i][0] -= k * v.back()[0];
  return Cnt(n, v) + std::max<i64>(0, v.back()[0] - n);
}

std::vector<std::tuple<int, i64, i64>> edge;
std::vector<i64> Rep(i64 n, std::vector<i64> v, std::vector<i64> s) {
  std::sort(ALL(v), std::greater());
  while (!v.back()) v.pop_back();
  if (v.back() >= n) return s;
  if (v.size() == 1) {
    for (i64 &w : s) w %= v[0];
    return s;
  }
  i64 k = (n - 1) / v.back();
  for (int i = 0; i + 1 < v.size(); ++i) k = std::min(k, v[i] / v.back());
  n -= k * v.back();
  for (int i = 0; i + 1 < v.size(); ++i) v[i] -= k * v.back();
  std::vector<i64> t, ans(s.size(), -1);
  for (int i = 0; i < s.size(); ++i) {
    i64 x = s[i];
    if (x >= n + v.back()) x -= (x - n) / v.back() * v.back();
    if (n <= x && x < v.back())
      ans[i] = x;
    else {
      if (x >= n) x -= (x - n + v.back()) / v.back() * v.back();
      t.push_back(x);
    }
  }
  auto y = Rep(n, v, t);
  for (int i = 0, c = 0; i < s.size(); ++i)
    if (ans[i] < 0) ans[i] = y[c++];
  return ans;
}
void Proc(int l, int r, std::vector<i64> s) {
  if (s.size() <= 1 || l > a) return;
  if (l == r) {
    Add(ans, i64(s.size() - 1) * (MOD - adj[l][0]));
    for (int i = 1; i < s.size(); ++i) edge.emplace_back(adj[l][0], s[0], s[i]);
    return;
  }
  int m = (l + r) >> 1;
  std::vector<int> od(s.size());
  std::vector<i64> rep = Rep(n, pre[m], s), rp;
  for (int i = 0; i < s.size(); ++i) od[i] = i;
  sort(ALL(od), [&](int u, int v) { return rep[u] < rep[v]; });
  for (int i = 0, j; i < s.size(); i = j) {
    for (j = i; j < s.size() && rep[od[j]] == rep[od[i]]; ++j);
    rp.push_back(s[od[i]]);
    std::vector<i64> lp;
    for (int k = i; k < j; ++k) lp.push_back(s[od[k]]);
    Proc(l, m, lp);
  }
  Proc(m + 1, r, rp);
}

std::map<i64, i64> anc;
i64 Anc(i64 o) { return o - anc[o] ? anc[o] = Anc(anc[o]) : o; }

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> a >> b;
  for (int i = 1; i <= a; ++i) std::cin >> adj[i][1] >> adj[i][0];
  for (int i = 1; i <= b; ++i) std::cin >> u[i] >> v[i] >> w[i];

  sort(PAR(adj, 1, a + 1)), cc[0] = n;
  for (int i = 1; i <= a; ++i) {
    pre[i] = pre[i - 1], pre[i].push_back(adj[i][1]);
    std::vector<std::array<i64, 2>> tmp(pre[i].size());
    for (int j = 0; j < pre[i].size(); ++j) tmp[j] = {pre[i][j], pre[i][j]};
    nxt.clear(), cc[i] = Cnt(n, tmp), pre[i].swap(nxt);
    Add(ans, (cc[i - 1] - cc[i]) % MOD * adj[i][0]);
  }

  std::vector<i64> all;
  for (int i = 1; i <= b; ++i) {
    edge.emplace_back(w[i], u[i], v[i]);
    all.push_back(u[i]), all.push_back(v[i]);
  }
  sort(ALL(all)), all.erase(unique(ALL(all)), end(all));
  Proc(1, a + 1, all), sort(ALL(edge));
  for (i64 i : all) anc[i] = i;
  for (auto [w, u, v] : edge)
    if (Anc(u) - Anc(v)) anc[Anc(u)] = Anc(v), Add(ans, w);

  std::cout << ans << '\n';

  return 0;
}