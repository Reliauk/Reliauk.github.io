#include <bits/stdc++.h>

#include "ds.h"

#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

void solve(int n, int lim1, int lim2) {
  std::vector<int> par(n - 1), val(n - 1);
  std::vector<std::vector<int>> sub(n), anc(n + 1);
  for (int i = 2; i <= n; ++i)
    if (query(i) == n - 1) sub[n - 1].push_back(i), anc[i].push_back(n - 1);
  for (int i = 1; i < n - 1; ++i) {
    exchange(i, n - 1);
    for (int j = 2; j <= n; ++j)
      if (query(j) == n - 1) sub[i].push_back(j), anc[j].push_back(i);
    exchange(i, n - 1);
  }
  for (int i = 2; i <= n; ++i) {
    int mx = anc[i][0];
    for (int e : anc[i])
      if (sub[e].size() < sub[mx].size()) mx = e;
    val[i - 2] = mx;
  }
  for (int i = 2; i <= n; ++i) {
    int e = val[i - 2];
    std::vector<bool> vis(n + 1, false);
    std::sort(ALL(sub[e]), [&](int u, int v) {
      return sub[val[u - 2]].size() > sub[val[v - 2]].size();
    });
    for (int u : sub[e]) {
      if (u == i || vis[u]) continue;
      par[u - 2] = i;
      for (int j : sub[val[u - 2]]) vis[j] = true;
    }
  }
  for (int &i : par)
    if (!i) i = 1;
  answer(par, val);
}