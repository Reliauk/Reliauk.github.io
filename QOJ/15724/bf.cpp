#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E6 + 5;

int n, m;

std::array<int, N> anc;
int Anc(int o) { return o - anc[o] ? anc[o] = Anc(anc[o]) : o; }
void Merge(int u, int v) { anc[Anc(u)] = Anc(v); }
bool Okay(int r, int s) {
  for (int i = 0; i < n * m; ++i) anc[i] = i;
  auto o = [&](int i, int j) { return i * m + j; };
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j) {
      if (i >= r && j >= s) Merge(o(i, j), o(i - r, j - s));
      if (i >= r && j + s < m) Merge(o(i, j), o(i - r, j + s));
      if (i + r < n && j >= s) Merge(o(i, j), o(i + r, j - s));
      if (i + r < n && j + s < m) Merge(o(i, j), o(i + r, j + s));
      if (i >= s && j >= r) Merge(o(i, j), o(i - s, j - r));
      if (i >= s && j + r < m) Merge(o(i, j), o(i - s, j + r));
      if (i + s < n && j >= r) Merge(o(i, j), o(i + s, j - r));
      if (i + s < n && j + r < m) Merge(o(i, j), o(i + s, j + r));
    }
  int cc = 0;
  for (int i = 0; i < n * m; ++i) cc += anc[i] == i;
  return cc == 1;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m, assert(n <= m);
  for (int r = 1; r <= m / 2; ++r)
    for (int s = 1; s <= r; ++s)
      assert(Okay(r, s) == (std::gcd(r + s, r - s) == 1 && r + s <= n));

  return 0;
}