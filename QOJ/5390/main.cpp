#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 300, MOD = 998244353;
std::mt19937 rng(20090627);
int Rand() { return std::uniform_int_distribution<int>(1, MOD - 1)(rng); }
void Add(int &x, i64 y) { x = (x + y) % MOD; }
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}
int Power(int a, int b) {
  int r = 1;
  for (; b; b >>= 1, a = i64(a) * a % MOD)
    if (b & 1) r = i64(r) * a % MOD;
  return r;
}
int Inv(int a) { return Power(a, MOD - 2); }

int n, m, k;
std::vector<std::vector<int>> adj;
void Edge(int u, int v) {
  int w = Rand();
  adj[u][v] = w, adj[v][u] = MOD - w;
}
int Rank() {
  int r = 0;
  for (int i = 0; i < k; ++i) {
    int j = r;
    while (j < k && !adj[j][i]) ++j;
    if (j == k) continue;
    std::swap(adj[r], adj[j]);
    int inv = Inv(adj[r][i]);
    for (int j = r + 1; j < k; ++j) {
      int rat = MOD - i64(inv) * adj[j][i] % MOD;
      for (int x = 0; x < k; ++x) Add(adj[j][x], i64(rat) * adj[r][x]);
    }
    ++r;
  }
  return r >> 1;
}

void Proc() {
  std::cin >> n >> m, k = n + n + m;
  adj = std::vector(k, std::vector(k, 0));
  for (int i = 0; i < n; ++i) {
    std::string s;
    std::cin >> s, Edge(i, i + n);
    for (int j = 0; j < m; ++j)
      if (s[j] == '1') Edge(i, j + n + n), Edge(i + n, j + n + n);
  }
  int ans = Rank();
  std::cout << ans - n << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}