#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 35, S = 905, INF = 1E9;
namespace flow {
struct E {
  int v, w, i;
  E(int v_, int w_, int i_) { v = v_, w = w_, i = i_; }
};
int n;
std::array<std::vector<E>, S> adj;
void Edge(int u, int v, int w) {
  adj[u].emplace_back(v, w, adj[v].size());
  adj[v].emplace_back(u, 0, adj[u].size() - 1);
}
std::array<int, S> arc, lev;
void Flow(int s, int t) {
  auto bfs = [&]() {
    for (int i = 0; i <= n; ++i) arc[i] = 0, lev[i] = -1;
    std::queue<int> q;
    lev[s] = 0, q.emplace(s);
    for (int u; q.size();) {
      u = q.front(), q.pop();
      for (auto [v, w, _] : adj[u])
        if (w && lev[v] < 0) lev[v] = lev[u] + 1, q.emplace(v);
    }
    return ~lev[t];
  };
  auto dinic = [&](auto self, int u, int f) {
    if (u == t || !f) return f;
    int ret = 0;
    for (int &i = arc[u]; i < adj[u].size(); ++i) {
      auto [v, w, j] = adj[u][i];
      if (!w || lev[v] - lev[u] - 1) continue;
      int r = self(self, v, std::min(f, w));
      f -= r, ret += r, adj[u][i].w -= r, adj[v][j].w += r;
      if (!f) break;
    }
    return ret;
  };
  while (bfs()) dinic(dinic, s, INF);
}
}  // namespace flow

int n;
std::array<std::string, N> grid;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  for (int i = 0; i < n; ++i) std::cin >> grid[i];

  flow::n = (n - 1) * (n - 1) + 1;
  for (int i = 0; i < n - 1; ++i)
    for (int j = 0; j < n - 1; ++j) {
      int id = i * (n - 1) + j;
      if ((i + j) & 1)
        flow::Edge(flow::n - 1, id, 2);
      else
        flow::Edge(id, flow::n, 2);
    }
  for (int i = 0; i < n - 1; ++i)
    for (int j = 1; j < n - 1; ++j) {
      int v = i * (n - 1) + j, u = v - 1;
      if ((i + j) & 1) std::swap(u, v);
      if (grid[i][j] != grid[i + 1][j]) flow::Edge(u, v, 1);
    }
  for (int i = 1; i < n - 1; ++i)
    for (int j = 0; j < n - 1; ++j) {
      int v = i * (n - 1) + j, u = v - n + 1;
      if ((i + j) & 1) std::swap(u, v);
      if (grid[i][j] != grid[i][j + 1]) flow::Edge(u, v, 1);
    }
  for (int i = 0; i < n - 1; ++i) {
    int u = i * (n - 1), v = flow::n;
    if (~i & 1) --v, std::swap(u, v);
    if (grid[i][0] != grid[i + 1][0]) flow::Edge(u, v, 1);
    u = i * (n - 1) + n - 2, v = flow::n;
    if (~(i + n - 2) & 1) --v, std::swap(u, v);
    if (grid[i].back() != grid[i + 1].back()) flow::Edge(u, v, 1);
    u = i, v = flow::n;
    if (~i & 1) --v, std::swap(u, v);
    if (grid[0][i] != grid[0][i + 1]) flow::Edge(u, v, 1);
    u = (n - 2) * (n - 1) + i, v = flow::n;
    if (~(i + n - 2) & 1) --v, std::swap(u, v);
    if (grid[n - 1][i] != grid[n - 1][i + 1]) flow::Edge(u, v, 1);
  }

  flow::Flow(flow::n - 1, flow::n);
  bool ok = true;
  for (auto [v, w, i] : flow::adj[flow::n - 1]) ok &= !w;
  for (auto [v, w, i] : flow::adj[flow::n]) ok &= !flow::adj[v][i].w;
  std::cout << (ok ? "YES\n" : "NO\n");

  return 0;
}