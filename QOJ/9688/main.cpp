#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int S = 100, INF = 1E9;
std::mt19937_64 rng(20090627);

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

const std::array<int, 8> maxn{0, 2, 4, 6, 11, 14, 21, 25};

int n, k;
std::array<int, 8> siz;
std::array<std::array<bool, 30>, 30> know;
std::array<std::array<std::array<int, 30>, 30>, 8> ext, tell;

bool suc;
void Dfs(int r, int u);
void Run(int r, int u, int s) {
  if (s < 0) {
    flow::Reset(), flow::n = n + siz[r] + 1;
    for (int i = 1; i <= n; ++i) flow::Edge(0, i, 1);
    for (int i = 0; i < siz[r]; ++i) flow::Edge(i + n + 1, flow::n, 1 << i);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < siz[r]; ++j)
        if (know[i][ext[r][u][j]]) flow::Edge(i + 1, j + n + 1, 1);
    if (flow::Flow(0, flow::n) < (1 << siz[r]) - 1) return;
    for (int i = 0; i < n; ++i)
      for (auto [v, w, _] : flow::adj[i + 1])
        if (v > n && !w) tell[r][i][u] = ext[r][u][v - n - 1];
    if (Dfs(r, u + 1); suc) return;
    for (int i = 0; i < n; ++i) tell[r][i][u] = -1;
    return;
  }
  if (~ext[r][u][s]) return Run(r, u, s - 1);
  std::vector<int> nxt;
  for (int i = 0; i < n; ++i)
    if (!know[u][i]) nxt.push_back(i);
  std::shuffle(ALL(nxt), rng);
  for (int v : nxt) {
    if (std::count(PAR(ext[r][u], 0, siz[r]), v)) continue;
    int cnt = 0;
    for (int i = 0; i < n; ++i) cnt += know[i][v];
    if (cnt < (1 << s)) continue;
    ext[r][u][s] = v;
    if (Run(r, u, s - 1); suc) return;
    ext[r][u][s] = -1;
  }
}
void Dfs(int r, int u) {
  if (r > k) return void(suc = true);
  if (u == n) {
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < siz[r]; ++j) know[i][ext[r][i][j]] = true;
    if (Dfs(r + 1, 0); suc) return;
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < siz[r]; ++j) know[i][ext[r][i][j]] = false;
    return;
  }
  Run(r, u, siz[r] - 1);
}

void Proc(std::vector<int> ini) {
  for (int i = 0; i < ini.size(); ++i) ext[1][i][0] = ini[i];
  Dfs(1, 0), assert(suc);
}

int init(int k) {
  n = maxn[::k = k];
  for (int i = 0; i < n; ++i) know[i][i] = true;
  for (int i = 1; i <= k; ++i)
    for (int u = 0; u < n; ++u)
      for (int v = 0; v < n; ++v) tell[i][u][v] = ext[i][u][v] = -1;
  if (k == 1) siz = {0, 1, 0, 0, 0, 0, 0, 0}, Proc({});
  if (k == 2) siz = {0, 1, 2, 0, 0, 0, 0, 0}, Proc({});
  if (k == 3) siz = {0, 1, 2, 2, 0, 0, 0, 0}, Proc({});
  if (k == 4)
    siz = {0, 1, 3, 3, 3, 0, 0, 0}, Proc({4, 0, 1, 1, 7, 1, 0, 4, 7, 7, 7});
  if (k == 5)
    siz = {0, 1, 3, 3, 3, 3, 0, 0}, Proc({13, 13, 13, 12, 12, 12, 11, 11, 11});
  if (k == 6)
    siz = {0, 1, 3, 4, 4, 4, 4, 0},
    Proc({13, 13, 13, 12, 12, 12, 11, 11, 11, 3, 1,
          6,  0,  7,  15, 4,  15, 9,  12, 4,  5});
  if (k == 7)
    siz = {0, 1, 3, 4, 4, 4, 4, 4},
    Proc({13, 13, 13, 12, 12, 12, 11, 11, 11, 6, 0, 7, 15, 4, 15, 9, 12, 4, 5});
  return n;
}
u32 send(int k, int n, int r, int i, const std::vector<int> &rec) {
  u32 s = rec[0] << i, t = 0;
  for (int rr = 1; rr < r; ++rr)
    for (int j = 0; j < siz[rr]; ++j) s |= (rec[rr] >> j & 1) << ext[rr][i][j];
  if (r == k + 1) return s;
  for (int j = 0; j < n; ++j)
    if (~tell[r][i][j]) t |= (s >> tell[r][i][j] & 1) << j;
  return t;
}