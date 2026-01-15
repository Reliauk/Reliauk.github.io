#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5, M = 2E6 + 5;

int n, m, t;
std::array<std::vector<int>, M> adj;
std::array<std::map<int, int>, N> le, gt;
void Infer(int u, int v) { adj[u].push_back(v); }
void Make(int u, int x) {
  if (!le[u][x]) le[u][x] = ++t;
  if (!gt[u][x]) gt[u][x] = ++t;
}

int d, c;
std::vector<int> anc;
std::array<bool, M> in;
std::array<int, M> dfn, low, scc;
void Dfs(int u) {
  dfn[u] = low[u] = ++d, anc.push_back(u), in[u] = true;
  for (int v : adj[u]) {
    if (!dfn[v])
      Dfs(v), low[u] = std::min(low[u], low[v]);
    else if (in[v])
      low[u] = std::min(low[u], dfn[v]);
  }
  if (dfn[u] - low[u]) return;
  for (int v = -++c; v - u; anc.pop_back())
    in[v = anc.back()] = false, scc[v] = c;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;

  for (int i, j, k, x; m; --m) {
    std::cin >> i >> j >> k >> x;
    for (int o : {i, j, k}) Make(o, x - 1), Make(o, x);
    for (int u : {i, j, k})
      for (int v : {i, j, k})
        if (u - v) Infer(le[u][x - 1], gt[v][x - 1]), Infer(gt[u][x], le[v][x]);
  }
  for (int i = 1; i <= n; ++i) {
    if (le[i].empty()) continue;
    for (auto it = begin(le[i]); next(it) != end(le[i]); ++it)
      Infer(it->second, next(it)->second);
    for (auto it = begin(gt[i]); next(it) != end(gt[i]); ++it)
      Infer(next(it)->second, it->second);
  }

  for (int i = 1; i <= t; ++i)
    if (!dfn[i]) Dfs(i);

  std::vector<int> ans;
  for (int i = 1; i <= n; ++i) {
    for (auto it = begin(le[i]); it != end(le[i]); ++it) {
      int x = it->first, u = it->second, v = gt[i][x];
      if (scc[u] == scc[v]) std::cout << "NO\n", exit(0);
      if (scc[u] < scc[v] && ans.size() < i) ans.push_back(std::max(1, x));
    }
    if (ans.size() < i) ans.push_back(1E9);
  }
  std::cout << "YES\n";
  for (int i = 0; i < n; ++i) std::cout << ans[i] << "\n "[i < n - 1];

  return 0;
}