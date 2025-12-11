#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 3E5 + 5;

int n, m;
std::array<i64, N> a, c, ans;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int i = 1; i <= n; ++i) std::cin >> a[i];
  for (int i = 1; i <= m; ++i) std::cin >> c[i];

  std::priority_queue<i64> pq;
  for (int i = n; i; --i) {
    ans[i] = ans[i + 1];
    for (int i = 1; i <= m; ++i) pq.push(-c[i]);
    for (; pq.size() && a[i] >= 0; pq.pop()) a[i] += pq.top(), ++ans[i];
    if (a[i] < 0) pq.push(a[i]), --ans[i];
  }

  for (int i = 1; i <= n; ++i) std::cout << ans[i] << "\n "[i < n];

  return 0;
}