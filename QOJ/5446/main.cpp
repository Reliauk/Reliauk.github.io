#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 205, S = 1E3 + 5, INF = 1E9;
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
void Reset() {
  for (int i = 0; i <= n; ++i) adj[i].clear();
  n = 0;
}
int Flow(int s, int t) {
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
  int flow = 0;
  while (bfs()) flow += dinic(dinic, s, INF);
  return flow;
}
}  // namespace flow

int n;
std::array<std::array<int, N>, N> a, p, pos;

void Proc() {
  std::cin >> n;
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j) std::cin >> a[i][j], pos[i][j] = 0;

  for (int c = 1; c <= n; ++c) {
    flow::Reset(), flow::n = n + n + 1;
    for (int i = 1; i <= n; ++i)
      flow::Edge(0, i, 1), flow::Edge(i + n, flow::n, 1);
    static std::array<std::map<int, int>, N> rep;
    for (int i = 1; i <= n; ++i) {
      rep[i].clear();
      for (int j = 1; j <= n; ++j)
        if (!pos[i][j] && !rep[i].count(a[i][j]))
          flow::Edge(a[i][j], i + n, 1), rep[i][a[i][j]] = j;
    }
    assert(flow::Flow(0, flow::n) == n);
    for (int i = 1; i <= n; ++i)
      for (auto [v, w, j] : flow::adj[i])
        if (v > n && !w)
          p[v - n][pos[v - n][rep[v - n][i]] = c] = rep[v - n][i];
  }

  std::cout << n * (n - 1) / 2 << '\n';
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j < i; ++j)
      std::cout << i << ' ' << p[i][j] << ' ' << j << ' ' << p[j][i] << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}