#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5;

int t, n;
std::array<std::vector<int>, N> adj;
void Dfs(int u, int fa) {
  for (int v : adj[u])
    if (v - fa) Dfs(v, u);
  std::cout << u << ' ';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> t >> n;

  if (t == 1) {
    for (int i = 1, u, v; i < n; ++i) {
      std::cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    Dfs(1, 0), std::cout << '\n';
  } else {
    std::vector<int> dis(n - 1);
    for (int &v : dis) std::cin >> v;
    reverse(ALL(dis));
    std::vector<int> ch{1};
    for (int i = 0; i < n - 1; ++i) {
      for (int j = 1; j < dis[i]; ++j) ch.pop_back();
      std::cout << ch.back() << ' ' << i + 2 << '\n', ch.push_back(i + 2);
    }
  }

  return 0;
}