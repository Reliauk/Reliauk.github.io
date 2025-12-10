#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using i128 = __int128;

const int N = 3E5 + 5;

int n, m;
std::vector<std::pair<int, i64>> ch;
std::array<i64, N> a, c, sa, sc, val, vmo, ans;

void Modify(int p, int it) {
  i64 sum = sc[it] - a[p + 1] + val[it] * c[it] + vmo[it];
  for (int i = 1; i < it; ++i)
    sum += val[i] * c[i] + vmo[i], val[i] = vmo[i] = 0;
  if (it <= m) val[it] = sum / c[it], vmo[it] = sum % c[it];
  for (int i = it + 1; i <= m; ++i) ++val[i];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int i = 1; i <= n; ++i) std::cin >> a[i], sa[i] = sa[i - 1] + a[i];
  for (int i = 1; i <= m; ++i) std::cin >> c[i], sc[i] = sc[i - 1] + c[i];

  ch = {{n, sa[n]}};
  for (int p = n - 1; ~p; --p) {
    for (; ch.size() >= 2; ch.pop_back()) {
      auto [a, b] = ch.back();
      auto [c, d] = end(ch)[-2];
      if (i128(p - a) * (d - b) < i128(c - a) * (sa[p] - b)) break;
    }
    i64 slop = (ch.back().second - sa[p]) / (ch.back().first - p);
    Modify(p, std::upper_bound(PAR(sc, 1, m + 1), slop) - data(sc));
    ch.emplace_back(p, sa[p]);
    i64 ans = 0, r = 0;
    for (int i = 1; i <= m; ++i)
      ans += (vmo[i] > r) + val[i], r += (vmo[i] > r) * c[i] - vmo[i];
    ::ans[p] = i64(n - p) * m - ans;
  }

  for (int i = 0; i < n; ++i) std::cout << ans[i] << "\n "[i < n - 1];

  return 0;
}