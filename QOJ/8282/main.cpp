#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 5E5 + 5, LOG = 20;
const i64 INF = 1E18;

int n;
i64 ans;
std::array<i64, N> a;
std::array<std::array<i64, N>, LOG> mn, mx;
i64 Min(int l, int r) {
  if (l > r) return INF;
  int k = std::__lg(r - l + 1);
  return std::min(mn[k][l], mn[k][r - (1 << k) + 1]);
}
i64 Max(int l, int r) {
  if (l > r) return 0;
  int k = std::__lg(r - l + 1);
  return std::max(mx[k][l], mx[k][r - (1 << k) + 1]);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  for (int i = 1; i <= n; ++i) std::cin >> a[i];

  mn[0].fill(INF);
  for (int i = 2; i < n; ++i) {
    if (a[i - 1] <= a[i] && a[i] >= a[i + 1])
      mx[0][i] = a[i] + std::min(a[i - 1], a[i + 1]);
    if (a[i - 1] >= a[i] && a[i] <= a[i + 1])
      mn[0][i] = a[i] + std::max(a[i - 1], a[i + 1]);
  }
  for (int d = 1; d < LOG; ++d)
    for (int i = 1; i + (1 << d) - 1 <= n; ++i) {
      mn[d][i] = std::min(mn[d - 1][i], mn[d - 1][i + (1 << (d - 1))]);
      mx[d][i] = std::max(mx[d - 1][i], mx[d - 1][i + (1 << (d - 1))]);
    }

  for (int l = 1; l < n; ++l) {
    int lo = l, hi = n;
    while (lo < hi) {
      int mi = (lo + hi + 1) >> 1;
      bool chk = Max(l + 1, mi - 1) < Min(l + 1, mi - 1);
      (chk ? lo : hi) = mi - !chk;
    }
    ans += lo - l;
  }

  std::cout << ans << '\n';

  return 0;
}