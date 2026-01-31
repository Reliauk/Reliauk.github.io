#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E4 + 5, MOD = 998244353;

int l, r, m;
std::array<int, N> pos;
std::array<std::vector<int>, N> adj;

int tm;
std::array<int, N> par, vis, pre;
void Dfs(int u, int fa) {
  if (vis[u] == tm) return;
  vis[u] = tm, pre[u] = fa;
  for (int v : adj[u]) {
    if (vis[v] == tm) continue;
    if (vis[v] = tm, pre[v] = u; par[v]) Dfs(par[v], v);
  }
}

void Proc() {
  std::cin >> l >> r >> m;
  for (int i = 1; i <= l + r; ++i) adj[i].clear();
  for (int u, v; m; --m)
    std::cin >> u >> v, adj[u].push_back(v + l), adj[v + l].push_back(u);
  for (int i = 1; i <= r; ++i) pos[i] = i + 1;

  tm = 0;
  for (int i = 1; i <= l; ++i) par[i] = 0;
  for (int i = 1; i <= l + r; ++i) vis[i] = 0;
  for (int i = r; i; --i) {
    ++tm, Dfs(l + i, 0);
    int j = 1;
    while (j <= l && (vis[j] - tm || par[j])) ++j;
    if (j <= l) {
      while (pre[j] - i - l) j = pre[par[j] = pre[j]];
      pos[i] = 1, par[j] = l + i;
      continue;
    }
    j = l + r;
    while (j > l + i && vis[j] - tm) --j;
    if (j == l + i) continue;
    pos[j - l] = i + 1, pos[i] = 1, j = pre[j];
    while (pre[j] - i - l) j = pre[par[j] = pre[j]];
    par[j] = l + i;
  }

  int ans = 0;
  for (int p = 1; p <= r; ++p)
    for (int q = p, f = 0; q <= r; ++q) {
      f += pos[q] <= p;
      ans = (ans + i64(f) * p % MOD * q % MOD * ((p ^ q) + 1)) % MOD;
    }
  std::cout << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}