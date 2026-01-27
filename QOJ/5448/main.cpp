#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E3 + 5, K = 2E5 + 5, MOD = 998244353;
void Add(int &x, i64 y) { x = (x + y) % MOD; }
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}

int a, n, m, k;
std::array<int, K> p;

std::array<std::array<int, N>, N> dp;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> a >> n >> m >> k;
  for (int i = 1; i <= a * k; ++i) std::cin >> p[i];

  int des = 0;
  for (int i = 1; i < a * k; ++i) des += p[i] > p[i + 1];
  if (des > m) std::cout << "0\n", exit(0);

  dp[k][des] = 1;
  for (int i = k; i < n; ++i)
    for (int j = 0; j <= m; ++j) {
      Add(dp[i + 1][j], i64(dp[i][j]) * (j + 1));
      Add(dp[i + 1][j + 1], i64(dp[i][j]) * (i * a - j));
    }

  std::cout << dp[n][m] << '\n';

  return 0;
}