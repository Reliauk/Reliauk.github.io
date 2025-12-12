#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r
#include "shop.h"

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

int ask(std::vector<int> a, std::vector<int> b) {
  return query(data(a), a.size(), data(b), b.size());
}
void find_price(int task_id, int n, int k, int ans[]) {
  for (int i = 0; i < n; ++i) ans[i] = 0;
  if (n == 1) return void(ans[0] = k);

  std::vector<int> ch{0}, z, res;
  if (task_id - 3) {
    for (int i = 1; i < n; ++i) res.push_back(i);
    while (res.size() > 1) {
      int a = res.back(), b = end(res)[-2];
      res.pop_back(), res.pop_back();
      if (!ask({a}, {b})) std::swap(a, b);
      if (ask({a, b}, {ch.back()}))
        z.push_back(a), res.push_back(b);
      else
        ch.push_back(b), res.push_back(a);
    }
    z.insert(end(z), ALL(ch));
  } else {
    z.resize(n), iota(ALL(z), 0);
    if (!ask({0}, {n - 1})) reverse(ALL(z));
    res = {z.back()}, z.pop_back();
  }

  int one = ask({z.back()}, {res[0]}) ? res[0] : z.back();
  int lo = 0, hi = z.size() - 1;
  while (lo < hi) {
    int mi = (lo + hi) >> 1;
    if (ask({z[mi], z[mi + 1]}, {one}))
      lo = mi + 1;
    else
      hi = mi;
  }

  for (int i = lo + 1; i < z.size(); ++i) ans[z[i]] = 1;
  k ^= (z.size() - lo - 1) & 1;
  int a = z[lo], b = res[0];
  if (!ask({a}, {b})) std::swap(a, b);
  if (ask({a, b}, {one}))
    ans[b] = k;
  else
    ans[b] = 1, ans[a] = k ^ 1;
}