#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 50, M = 152;

int mod;
std::array<int, M> inv{0, 1};
void Add(int &x, i64 y) { x = (x + y) % mod; }
void Add(int &x, int y) {
  if ((x += y) >= mod) x -= mod;
}
void Math() {
  for (int i = 2; i < M; ++i) inv[i] = i64(mod - mod / i) * inv[mod % i] % mod;
}

int n, m, k;
int f[M][M][N][N];

int h[N][M], h1[M][N][M], h2[M][N][N], h3[M][M][N][N];
void Dp(int n, int a, int b, int x, int y) {
  int w = f[a][b][x][y];
  Add(h1[a][x][a + std::max(0, k - b) + 1], w);
  if (k > b)
    Add(h3[a][k - b][x][y + 1], w);
  else if (b > k)
    Add(h2[a][x][x + 1], w);
  else
    Add(h2[a][x][std::max(x, y) + 1], w);
  f[a][b][x][y] = 0;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m >> k >> mod, Math();
  if (n * k > 2 * (m + k)) std::cout << "0\n", exit(0);

  for (int i = 0; i < m; ++i) f[i][k + 1][0][0] = 1;
  for (int i = 1; i < n; ++i) {
    for (int a = 0; a < m; ++a)
      for (int b = 0; b <= k + 1; ++b)
        for (int x = 0; x < i; ++x)
          for (int y = 0; y < i; ++y)
            if (f[a][b][x][y]) Dp(i + 1, a, b, x, y);
    for (int a = 0; a < m; ++a)
      for (int x = 0; x < i; ++x) {
        int up = std::min(m, a + k + 1), p = a + 1;
        while (p < m && !h1[a][x][p]) ++p;
        up = std::max(up, p);
        for (int c = p; c < up; ++c) {
          for (int y = 0; y <= i; ++y)
            Add(f[c][std::min(k + 1, c - a)][y][x + (y <= x)], h1[a][x][c]);
          Add(h1[a][x][c + 1], h1[a][x][c]), h1[a][x][c] = 0;
        }
        for (int c = up; c < m; ++c) {
          Add(h[x][c], h1[a][x][c]), Add(h1[a][x][c + 1], h1[a][x][c]);
          h1[a][x][c] = 0;
        }
        for (int y = x + 1; y <= i; ++y) {
          Add(h2[a][x][y], h2[a][x][y - 1]);
          Add(f[a][0][y][x], h2[a][x][y]);
        }
        memset(h2[a][x], 0, 4 * N);
      }
    for (int x = 0; x < i; ++x)
      for (int c = 0; c < m; ++c)
        if (h[x][c]) {
          for (int y = 0; y <= i; ++y)
            Add(f[c][k + 1][y][x + (y <= x)], h[x][c]);
          h[x][c] = 0;
        }
    for (int a = 0; a < m; ++a)
      for (int b = 1; b <= k && a + b < m; ++b)
        for (int x = 0; x < i; ++x) {
          int p = 0;
          while (p <= i && !h3[a][b][x][p]) ++p;
          for (int y = p; y <= i; ++y) {
            Add(f[a + b][b][y][x + (y <= x)], h3[a][b][x][y]);
            if (y < i) Add(h3[a][b][x][y + 1], h3[a][b][x][y]);
            h3[a][b][x][y] = 0;
          }
        }
  }

  int ans = 0;
  for (int a = 0; a < m; ++a)
    for (int b = 0; b <= k + 1; ++b)
      for (int x = 0; x < n; ++x)
        for (int y = 0; y < n; ++y) Add(ans, f[a][b][x][y]);
  for (int i = 1; i <= n; ++i) ans = i64(ans) * inv[m] % mod;

  std::cout << ans << '\n';

  return 0;
}