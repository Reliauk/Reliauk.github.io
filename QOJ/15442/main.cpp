#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 5E5 + 5;

int n, v;
std::array<int, N> a, nxt, rk;
std::array<std::vector<int>, N> pos;

void Proc() {
  std::cin >> n, v = n;
  for (int i = 1; i <= n; ++i) pos[i].clear();
  for (int i = 1; i <= n; ++i) std::cin >> a[i], pos[a[i]].push_back(i);

  std::vector<int> stk;
  for (int i = n; i; --i) {
    while (stk.size() && a[stk.back()] <= a[i]) stk.pop_back();
    stk.push_back(i);
  }
  for (int i = n; i; --i) {
    while (stk.size() && a[stk.back()] <= a[i]) stk.pop_back();
    nxt[i] = stk.size() ? stk.back() : 0, stk.push_back(i);
  }

  for (int i = n; i; --i) {
    if (pos[i].empty()) continue;
    if (v == n) {
      for (int j : pos[i]) rk[j] = v--;
      continue;
    }
    std::sort(ALL(pos[i]), [&](int p, int q) {
      int lp = (nxt[p] - p + n) % n;
      int lq = (nxt[q] - q + n) % n;
      if (lp - lq) return lp < lq;
      return rk[nxt[p]] > rk[nxt[q]];
    });
    for (int j : pos[i]) rk[j] = v--;
  }

  int p = 1;
  while (rk[p] - 1) ++p;
  for (int i = 0, cur = 0; i < n; ++i)
    std::cout << (cur = std::max(cur, a[(p + i - 1) % n + 1])) << ' ';
  std::cout << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}