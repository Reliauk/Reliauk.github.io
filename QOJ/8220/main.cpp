#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 5E4 + 5, L = 16;
std::mt19937_64 rng(20090627);

int n, s, t;
std::array<std::vector<int>, N> adj;

int d;
std::array<int, N> dfn, dep;
std::array<std::array<int, N>, L> st;
int Min(int u, int v) { return dfn[u] < dfn[v] ? u : v; }
void Dfs(int u, int fa) {
  dep[u] = dep[st[0][dfn[u] = ++d] = fa] + 1;
  for (int v : adj[u])
    if (v - fa) Dfs(v, u);
}
int Lca(int u, int v) {
  if (u == v) return u;
  auto [l, r] = std::minmax(dfn[u], dfn[v]);
  int k = std::__lg(r - l);
  return Min(st[k][l + 1], st[k][r - (1 << k) + 1]);
}
int Dis(int u, int v) { return dep[u] + dep[v] - 2 * dep[Lca(u, v)]; }

void Proc() {
  std::cin >> n >> s >> t;
  for (int i = 1; i <= n; ++i) adj[i].clear();
  for (int i = 1, u, v; i < n; ++i)
    std::cin >> u >> v, adj[u].push_back(v), adj[v].push_back(u);

  d = 0, Dfs(1, 0);
  for (int d = 1; d < L; ++d)
    for (int i = 1; i + (1 << d) - 1 <= n; ++i)
      st[d][i] = Min(st[d - 1][i], st[d - 1][i + (1 << (d - 1))]);

  if (n == 2) return void(std::cout << s << ' ' << t << '\n');
  std::vector<int> p{s};
  for (int i = 1; i <= n; ++i)
    if (i - s && i - t) p.push_back(i);
  p.push_back(t), std::shuffle(PAR(p, 1, n - 1), rng);

  int val = 0;
  for (int i = 1; i < n; ++i) val ^= Dis(p[i - 1], p[i]);
  auto q = p;
  for (int it = n * 500; n > 3 && it && val > 1; --it) {
    if (val < 4) q = p;
    int i = rng() % (n - 3) + 1;
    val ^= Dis(p[i - 1], p[i]) ^ Dis(p[i + 1], p[i + 2]);
    std::swap(p[i], p[i + 1]);
    val ^= Dis(p[i - 1], p[i]) ^ Dis(p[i + 1], p[i + 2]);
  }
  if (val < 2) q = p;
  for (int x : q) std::cout << x << ' ';
  std::cout << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}