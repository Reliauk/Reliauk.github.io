#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5;

int n;
i64 ans;
std::array<std::array<int, N>, 3> a;

std::array<int, N> od, tmp, bit;
std::array<std::array<int, N>, 4> cnt;
void Add(int x, int v) {
  for (; x <= n; x += x & -x) bit[x] += v;
}
int Query(int x) {
  int v = 0;
  for (; x; x &= x - 1) v += bit[x];
  return v;
}
void Proc(int l, int r, int o) {
  if (l + 1 == r) return;
  int m = (l + r) >> 1, p = l;
  Proc(l, m, o), Proc(m, r, o);
  for (int i = m; i < r; ++i) {
    while (p < m && a[1][od[p]] < a[1][od[i]]) Add(a[2][od[p]], 1), ++p;
    cnt[o][od[i]] += Query(a[2][od[i]] - 1);
  }
  while (--p >= l) Add(a[2][od[p]], -1);
  std::merge(PAR(od, l, m), PAR(od, m, r), data(tmp) + l,
             [&](int u, int v) { return a[1][u] < a[1][v]; });
  for (int i = l; i < r; ++i) od[i] = tmp[i];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  for (int i = 1; i <= n; ++i)
    for (int c : {0, 1, 2}) std::cin >> a[c][i];
  ans = i64(n - 2) * (n - 1) * n / 6 + i64(n - 1) * n / 2 + n;

  for (int c : {0, 1, 2, 3}) {
    if (c < 3)
      for (int i = 1; i <= n; ++i) a[c][i] = n + 1 - a[c][i];
    for (int i = 1; i <= n; ++i) od[i] = i;
    std::sort(PAR(od, 1, n + 1),
              [&](int u, int v) { return a[0][u] < a[0][v]; });
    Proc(1, n + 1, c);
    if (c < 3)
      for (int i = 1; i <= n; ++i) a[c][i] = n + 1 - a[c][i];
  }

  for (int i = 1; i <= n; ++i) {
    ans -= i64(cnt[3][i] + 1) * cnt[3][i] / 2;
    for (int c : {0, 1, 2})
      ans -= i64(cnt[3][i]) * cnt[c][i] + i64(cnt[c][i] - 1) * cnt[c][i] / 2;
  }
  std::cout << ans << '\n';

  return 0;
}