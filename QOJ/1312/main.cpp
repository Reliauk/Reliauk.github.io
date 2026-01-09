#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E6 + 5;

int n, m;
std::array<int, N> a, b;
std::array<std::vector<std::pair<int, int>>, N> adj;

int sa, sb;
std::vector<int> cur;
std::pair<int, int> ext;
std::array<bool, N> vis;
void Dfs(int u, int fa) {
  vis[u] = true, cur.push_back(u);
  for (auto [v, w] : adj[u])
    if (!vis[v]) {
      Dfs(v, u);
      if (u < v)
        sb += b[w];
      else
        sa += a[w];
    } else if (v - fa)
      ext = {v, u};
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int i = 1, x, y; i <= m; ++i) {
    std::cin >> x >> y >> a[i] >> b[i];
    adj[x].emplace_back(y + n, i);
    adj[y + n].emplace_back(x, i);
  }

  for (int i = 1; i <= n + n; ++i) {
    if (vis[i]) continue;
    cur.clear(), ext = {0, 0}, sa = sb = 0, Dfs(i, 0);
    if (ext.first) {
    } else {
    }
  }

  return 0;
}