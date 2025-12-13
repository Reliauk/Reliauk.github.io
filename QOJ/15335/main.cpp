#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5;

int n, m, c;
std::array<std::vector<int>, N> adj;

int d;
std::vector<int> anc;
std::array<int, N * 2> dfn, low, dep;
std::array<std::vector<int>, N * 2> bf;
void Dfs(int u, int fa) {
  dfn[u] = low[u] = ++d, anc.push_back(u);
  for (int v : adj[u])
    if (!dfn[v]) {
      Dfs(v, u), low[u] = std::min(low[u], low[v]);
      if (low[v] < dfn[u]) continue;
      bf[++c].push_back(u), bf[u].push_back(c);
      for (int w = 0; w - v; anc.pop_back())
        bf[w = anc.back()].push_back(c), bf[c].push_back(w);
    } else
      low[u] = std::min(low[u], dfn[v]);
}
void Get(int u, int fa) {
  dep[u] = dep[fa] + 1;
  for (int v : bf[u])
    if (v - fa) Get(v, u);
}
void Mark(int u, int fa) {
  dfn[u] = ++d;
  for (int v : bf[u])
    if (v - fa) Mark(v, u);
}
int s, t;
bool Chk(int u, int fa) {
  bool sp = u == t;
  for (int v : bf[u])
    if (v - fa) sp |= Chk(v, u);
  if (!sp && u > n) std::cout << "IMPOSSIBLE\n", exit(0);
  return sp;
}

std::array<int, N> fa;
std::array<std::vector<int>, N> ext;
void Spt(int u, int fa) {
  dep[u] = dep[::fa[u] = fa] + 1, anc.push_back(u);
  for (int v : adj[u])
    if (!dep[v])
      Spt(v, u);
    else if (dep[v] < dep[u] && v - fa)
      ext[anc[dep[v]]].push_back(u);
  anc.pop_back();
}

std::array<int, N> deg;
std::array<bool, N> vis;
std::queue<std::pair<int, int>> ori;
std::array<std::vector<int>, N> dag;
void Edge(int u, int v, int w) {
  dag[w ? u : v].push_back(w ? v : u), ++deg[w ? v : u];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int u, v; m; --m)
    std::cin >> u >> v, adj[u].push_back(v), adj[v].push_back(u);

  c = n, Dfs(1, 0), d = 0, Mark(1, 0);
  s = std::max_element(PAR(dfn, n + 1, c + 1)) - data(dfn);
  Get(s, 0);
  t = std::max_element(PAR(dep, n + 1, c + 1)) - data(dep);
  Chk(s, 0);
  for (int v : bf[s])
    if (bf[v].size() == 1) {
      s = v;
      break;
    }
  for (int v : bf[t])
    if (bf[v].size() == 1 && v - s) {
      t = v;
      break;
    }

  dep.fill(0), anc.clear(), Spt(s, -1), ori.emplace(t, 1);
  while (ori.size()) {
    auto [u, d] = ori.front();
    ori.pop();
    for (; u - s && !vis[u]; u = fa[u]) {
      vis[u] = true, Edge(fa[u], u, d);
      for (int v : ext[u]) Edge(fa[u], v, d), ori.emplace(v, !d);
    }
  }

  std::queue<int> bfs;
  for (int i = 1; i <= n; ++i)
    if (!deg[i]) bfs.push(i);
  for (int u; bfs.size();) {
    std::cout << (u = bfs.front()) << ' ', bfs.pop();
    for (int v : dag[u])
      if (!--deg[v]) bfs.push(v);
  }
  std::cout << '\n';

  return 0;
}