#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5;
std::mt19937_64 rng(20090627);

int n;
std::vector<int> ans;
std::array<int, N> a, q, f, g;
std::unordered_map<u64, int> occ;
std::array<u64, N> w, s;

std::array<int, N> bit;
std::array<i64, N> inv;
void Add(int x, int v) {
  for (; x <= n; x += x & -x) bit[x] += v;
}
int Query(int x) {
  int v = 0;
  for (; x; x &= x - 1) v += bit[x];
  return v;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  for (int i = 1; i <= n; ++i) std::cin >> a[i], w[i] = rng(), q[a[i]] = i;
  for (int i = 1; i <= n; ++i) s[i] = s[i - 1] ^ w[i] ^ w[a[i]];

  for (int i = 1; i <= n; ++i)
    Add(a[i], 1), inv[i] = i - Query(a[i]) + inv[i - 1];
  occ[0] = 0, bit.fill(0);
  for (int i = 1; i <= n; ++i) {
    f[i] = f[i - 1], g[i] = i - 1, Add(q[i], 1);
    if (occ.count(s[i])) {
      int j = occ[s[i]];
      if (f[j] >= f[i] &&
          inv[i] - inv[j] - i64(j - Query(j)) * (i - j) == i - j - 1)
        f[i] = f[j] + 1, g[i] = j;
    }
    occ[s[i]] = i;
  }

  for (int cur = n; cur; cur = g[cur]) {
    if (f[g[cur]] == f[cur]) continue;
    int l = g[cur] + 1, r = cur;
    for (int i = l; i <= r; ++i) {
      int j = i;
      while (a[j] - i) ++j;
      while (j > i) std::swap(a[j - 1], a[j]), ans.push_back(--j);
    }
  }
  std::cout << ans.size() << '\n';
  for (int x : ans) std::cout << x << ' ';
  std::cout << '\n';

  return 0;
}