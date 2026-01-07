#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 5E5 + 5;

int n, m;
std::array<std::vector<int>, N> adj, jda;

std::vector<int> cyc;
std::pair<int, int> back;
std::array<int, N> fa, onc;
std::array<bool, N> vis, anc;
void Dfs(int u) {
  vis[u] = anc[u] = true;
  for (int v : adj[u])
    if (!vis[v])
      fa[v] = u, Dfs(v);
    else if (anc[v])
      back = {u, v};
  anc[u] = false;
}
void Cyc() {
  for (int i = 1; i <= n; ++i) {
    if (vis[i]) continue;
    back = {0, 0}, Dfs(i);
    auto [u, v] = back;
    if (u) {
      for (; u - v; u = fa[u]) cyc.push_back(u);
      return cyc.push_back(v), reverse(ALL(cyc));
    }
  }
  std::cout << "NIE\n", exit(0);
}

std::queue<int> topo;
std::array<int, N> deg;
std::array<std::vector<int>, N> subg;

std::array<int, N> cov, max, min;
int Min(int u) {
  if (onc[u] >= 0) return onc[u];
  if (min[u] <= n) return min[u];
  min[u] = n;
  for (int v : adj[u]) min[u] = std::min(min[u], Min(v));
  return min[u];
}
int Max(int u) {
  if (onc[u] >= 0) return onc[u];
  if (max[u] > -2) return max[u];
  max[u] = -1;
  for (int v : adj[u]) max[u] = std::max(max[u], Max(v));
  return max[u];
}
int Max2(int u) {
  if (onc[u] >= 0) return onc[u];
  if (max[u] > -2) return max[u];
  max[u] = -1;
  for (int v : jda[u]) max[u] = std::max(max[u], Max2(v));
  return max[u];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int u, v; m; --m)
    std::cin >> u >> v, adj[u].push_back(v), jda[v].push_back(u);

  Cyc(), onc.fill(-1);
  for (int i = 0; i < cyc.size(); ++i) onc[cyc[i]] = i;
  for (int i = 1; i <= n; ++i) {
    if (~onc[i]) continue;
    for (int j : adj[i])
      if (onc[j] < 0) subg[i].push_back(j), ++deg[j];
  }
  for (int i = 1; i <= n; ++i)
    if (onc[i] < 0 && !deg[i]) topo.push(i);
  for (int u; topo.size();) {
    u = topo.front(), topo.pop();
    for (int v : subg[u])
      if (!--deg[v]) topo.push(v);
  }
  for (int i = 1; i <= n; ++i)
    if (onc[i] < 0 && deg[i] > 0) std::cout << "0\n", exit(0);

  max.fill(-2), min.fill(n + 1);
  for (int i = 0; i < cyc.size(); ++i) {
    int mx = -1, mn = n, nxt = cyc[(i + 1) % cyc.size()];
    for (int v : adj[cyc[i]])
      if (v - nxt) mx = std::max(mx, Max(v)), mn = std::min(mn, Min(v));
    if (i < mx) ++cov[i + 1], --cov[mx];
    if (mn <= i) ++cov[i + 1];
  }
  max.fill(-2);
  for (int i = 0; i < cyc.size(); ++i) {
    int mx = -1, prv = cyc[(i + cyc.size() - 1) % cyc.size()];
    for (int v : jda[cyc[i]])
      if (v - prv) mx = std::max(mx, Max2(v));
    if (mx >= i) ++cov[0], --cov[i];
  }

  std::vector<int> ans;
  for (int i = 0; i < cyc.size(); ++i) {
    if (i) cov[i] += cov[i - 1];
    if (!cov[i]) ans.push_back(cyc[i]);
  }
  sort(ALL(ans)), std::cout << ans.size() << '\n';
  for (int u : ans) std::cout << u << ' ';
  std::cout << '\n';

  return 0;
}