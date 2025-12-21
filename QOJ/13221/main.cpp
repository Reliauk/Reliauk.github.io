#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5, MOD = 998244353;
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

int m, k;
std::array<int, N> f, g;
void Proc() {
  std::cin >> m >> k;
  for (int i = 1; i <= k; ++i) f[i] = Power(i + i + 1, m) - 1, g[i] = 0;
  for (int i = 1; i <= k; ++i) {
    Add(g[i], g[i - 1]), Add(f[i], g[i]);
    for (int d = 2; d <= k / i; ++d) {
      Add(g[i * d], MOD - f[i]);
      if ((i + 1) * d <= k) Add(g[(i + 1) * d], f[i]);
    }
  }
  Add(f[k], 1), std::cout << f[k] << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}