#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5;

int n, m;
std::array<int, N> ans;
std::array<std::vector<int>, N> adj;

std::array<int, N> dep, fa;
std::array<std::set<int>, N> spt;
std::array<std::vector<int>, N> anc, giv;
void Dfs(int u, int fa) {
  dep[u] = dep[fa] + 1, ::fa[u] = fa;
  for (int v : adj[u])
    if (dep[v] < 0)
      Dfs(v, u), spt[u].insert(v);
    else if (v - fa)
      anc[v].push_back(u);
}

void Give(int u) {
  for (int v : giv[u]) ans[v] = ans[u], Give(v);
}

int dbg = 0;

void Proc() {
  ++dbg;
  std::cin >> n >> m;
  for (int i = 1; i <= n; ++i) adj[i].clear();
  for (int i = 0, u, v; i < m; ++i) {
    std::cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  if (n & 1) return void(std::cout << "-1\n");

  for (int i = 1; i <= n; ++i)
    ans[i] = 0, dep[i] = -1, spt[i].clear(), anc[i].clear(), giv[i].clear();
  Dfs(1, 0);
  std::set<std::pair<int, int>> al;
  for (int i = 1; i <= n; ++i) {
    al.emplace(dep[i], i);
    sort(ALL(anc[i]), [&](int u, int v) { return dep[u] < dep[v]; });
  }

  int col = 0;
  while (al.size()) {
    int v = rbegin(al)->second, u = fa[v];
    if (spt[u].size() & 1) {
      ans[u] = ++col, al.erase({dep[u], u});
      for (int v : spt[u]) ans[v] = col, al.erase({dep[v], v});
      if (fa[u]) spt[fa[u]].erase(u);
      spt[u].clear();
      continue;
    }
    int key = 0;
    for (int v : spt[u]) {
      while (anc[v].size() && !al.count({dep[anc[v].back()], anc[v].back()}))
        anc[v].pop_back();
      if (anc[v].size()) key = v;
    }
    if (!key) {
      for (int v : spt[u]) giv[u].push_back(v), al.erase({dep[v], v});
      spt[u].clear();
      continue;
    }
    al.erase({dep[key], key});
    dep[key] = dep[fa[key] = anc[key].back()] + 1;
    spt[fa[key]].insert(key), anc[key].pop_back();
    ans[u] = ++col, al.erase({dep[u], u});
    if (fa[u]) spt[fa[u]].erase(u);
    for (int v : spt[u])
      if (v - key) ans[v] = col, al.erase({dep[v], v});
    al.emplace(dep[key], key), spt[u].clear();
  }

  std::vector<int> rts;
  for (int i = 1; i <= n; ++i)
    if (ans[i]) rts.push_back(i);
  for (int i : rts) Give(i);

  for (int i = 1; i <= n; ++i) std::cout << ans[i] << "\n "[i < n];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}