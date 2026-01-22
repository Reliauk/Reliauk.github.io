#include <bits/stdc++.h>

#include "prg.h"
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

bool Check(int z, int o, double p) {
  double prob = std::log(p) * o + std::log(1 - p) * z;
  return std::log(0.5) * (z + o) < prob;
}
int solve(int n, int m, int k, std::vector<int> op,
          std::array<std::vector<int>, 3000> x, int c,
          std::array<std::array<int, 3000>, 25> s) {
  std::vector<double> p{0.5};
  for (int i : op)
    if (i == 0)
      p.push_back(p.back() / 2);
    else if (i == 1)
      p.push_back(p.back() + (1 - p.back()) / 2);
    else
      p.push_back(0.5);

  if (op.back() - 2) {
    int z = 0, o = 0;
    for (auto &v : s) z += std::count(ALL(v), 0), o += std::count(ALL(v), 1);
    return 1 + Check(z, o, p[k - 1]);
  }

  if (op[k - 3] - 2) {
    std::vector<std::vector<int>> grp(n);
    for (int i = 0; i < m; ++i) grp[x[i].back()].push_back(i);
    int z = 0, o = 0;
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < c; ++j)
        for (int k = 1; k < grp[i].size(); ++k)
          ++(s[j][grp[i][k]] - s[j][grp[i][0]] ? o : z);
    return 1 + Check(z, o, p[k - 2] * (1 - p[k - 2]) * 2);
  }

  std::vector<std::vector<std::pair<int, int>>> adj(n);
  for (int i = 0; i < m; ++i) {
    adj[x[i][2]].emplace_back(x[i][3], i);
    adj[x[i][3]].emplace_back(x[i][2], i);
  }
  std::vector<int> cur;
  std::set<std::vector<int>> cyc;
  auto dfs = [&](auto self, int u, int st) {
    if (u == st && cur.size() >= 2) cyc.emplace(cur);
    if (cur.size() == 5) return;
    for (auto [v, w] : adj[u])
      if (cur.empty() || cur.back() < w)
        cur.push_back(w), self(self, v, st), cur.pop_back();
  };
  for (int i = 0; i < n; ++i) dfs(dfs, i, i);

  std::array<double, 6> po;
  po[2] = p[1] * (1 - p[1]) * 2;
  po[3] = p[1] * (1 - p[1]) * (1 - p[1]) * 3 + p[1] * p[1] * p[1];
  po[4] = p[1] * std::pow(1 - p[1], 3) * 4 + std::pow(p[1], 3) * (1 - p[1]) * 4;
  po[5] = p[1] * std::pow(1 - p[1], 4) * 5 +
          std::pow(p[1], 3) * (1 - p[1]) * (1 - p[1]) * 10 + std::pow(p[1], 5);

  double lp = 0, rp = 0;
  for (auto path : cyc)
    for (int i = 0; i < c; ++i) {
      int val = 0;
      for (int j : path) val ^= s[i][j];
      lp += std::log(0.5);
      rp += std::log(val ? po[path.size()] : 1 - po[path.size()]);
    }

  return 1 + (lp < rp);
}