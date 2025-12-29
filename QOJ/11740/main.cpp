#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E6 + 5;

int n, m, a, b;
std::array<std::vector<std::pair<int, int>>, N> adj;

std::array<int, N> ans;
std::array<bool, N> vis;
std::tuple<int, int, int> ext;
std::vector<int> cur;
void Dfs(int u, int fa) {
  vis[u] = true, cur.push_back(u);
  for (auto [v, i] : adj[u])
    if (!vis[v])
      Dfs(v, u);
    else if (v - fa)
      ext = {u, v, i};
}
void Mark(int u) {
  vis[u] = true;
  for (auto [v, i] : adj[u])
    if (!vis[v]) ans[v] = i, Mark(v);
}

void Proc() {
  for (int i = 1; i <= n + n; ++i) adj[i].clear(), vis[i] = false;
  for (int i = 1, x, y; i <= m; ++i) {
    std::cin >> x >> y;
    if (x == b && y == a) continue;
    int u = x == b ? y + n : x;
    int v = y == a ? x : y + n;
    adj[u].emplace_back(v, i);
    adj[v].emplace_back(u, i);
  }
  for (int i = 1; i <= n + n; ++i) {
    if (vis[i] || i == a + n || i == b) continue;
    cur.clear(), ext = {-1, -1, -1}, Dfs(i, 0);
    if (std::get<0>(ext) < 0) return void(std::cout << "NO\n");
    for (int i : cur) vis[i] = false;
    Mark(std::get<0>(ext)), ans[std::get<0>(ext)] = std::get<2>(ext);
  }
  std::cout << "YES\n";
  for (int i = 1; i <= n; ++i)
    if (i - a) std::cout << ans[i + n] << ' ';
  std::cout << '\n';
  for (int i = 1; i <= n; ++i)
    if (i - b) std::cout << ans[i] << ' ';
  std::cout << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  while (std::cin >> n >> m >> a >> b) Proc();

  return 0;
}