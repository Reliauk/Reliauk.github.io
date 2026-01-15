#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 3E3 + 5, S = 1E5 + 5, INF = 1E9;

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

int n, m;
std::array<int, N> a, hd;

void Proc() {
  std::cin >> n >> m, flow::Reset(), flow::n = n;
  for (int i = 1; i <= n; ++i) std::cin >> a[i], flow::Edge(0, hd[i] = i, 1);
  for (int x, y; m; --m) {
    std::cin >> x >> y;
    if (x == y) continue;
    int u = ++flow::n, v = ++flow::n;
    flow::Edge(hd[x], hd[y], 1), flow::Edge(hd[y], hd[x], 1);
    flow::Edge(hd[x], u, a[x]), flow::Edge(hd[y], v, a[y]);
    hd[x] = u, hd[y] = v;
  }
  flow::Edge(hd[1], ++flow::n, a[1]);
  std::cout << flow::Flow(0, flow::n) << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}