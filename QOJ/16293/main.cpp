#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

i64 vl, vr;
i64 F(i64 t, i64 n) {
  if (n < 0) return 0;
  if (n >= t / 2) return std::max<i64>(0, std::min(n, vr) - vl + 1);
  i64 x = std::max<i64>(0, (t / 2 - n) / 3 * 2 - 2);
  t -= x, n += x;
  while (n < t / 2) ++n, --t, ++x;
  return F(t, n) - F(t, x - 1);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int q;
  std::cin >> q;
  for (i64 l, r, t; q; --q) {
    std::cin >> vl >> vr >> l >> r >> t;
    std::cout << F(t, r) - F(t, l - 1) << '\n';
  }

  return 0;
}