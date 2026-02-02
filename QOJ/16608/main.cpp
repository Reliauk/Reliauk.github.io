#include <bits/stdc++.h>

#include "paper_mills.h"

#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using LL = long long;

auto solve_paper_mills(int n, std::vector<int> a, int m, std::vector<int> d)
    -> std::vector<LL> {
  i64 sum = 0;
  std::priority_queue<i64> pq;
  for (int i = 0; i < n; ++i) pq.push(-a[i]);
  for (i64 o; pq.size() > 1;)
    o = pq.top(), pq.pop(), o += pq.top(), pq.pop(), sum -= o, pq.push(o);

  std::array<i64, 50> memo;
  std::vector<i64> cur(ALL(a));
  memo.fill(0), std::sort(ALL(cur));
  for (int d = 1; d < 50; ++d) {
    std::vector<i64> nxt;
    for (int i = 0; i + 1 < cur.size(); i += 2)
      nxt.push_back(cur[i] + cur[i + 1]);
    if (nxt.size() >= n - 1)
      for (int i = 0; i < n - 1; ++i) memo[d] += nxt[i];
    cur.swap(nxt), cur.insert(end(cur), ALL(a)), std::sort(ALL(cur));
  }

  std::vector<LL> ans(m);
  for (int i = 0; i < m; ++i)
    if (d[i] >= 50)
      ans[i] = sum;
    else
      ans[i] = memo[d[i]];
  return ans;
}