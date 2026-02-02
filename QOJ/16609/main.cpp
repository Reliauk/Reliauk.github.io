#include <bits/stdc++.h>

#include "non.h"

#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r
#define GET(t, i) std::get<i>(t)

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int INF = 2E9;

std::vector<int> distribute_non(int n, int m, std::vector<int> a) {
  std::vector<int> ans(m, INF), minr(m, INF), lpp(m);
  auto nxt_l = [&](int x, int l) { return l ? x / (x / l + 1) : 0; };
  auto dif_l = [&](int x, int l) {
    if (!l) return INF;
    return (nxt_l(x, l) ? x / nxt_l(x, l) : INF) - x / l;
  };
  auto nxt_r = [&](int x, int r) {
    if (r == 1 || x == 1 || r == 0) return 0;
    return (x - 1) / ((x + r - 1) / r);
  };
  auto dif_r = [&](int x, int r) {
    if (!r) return INF;
    return (nxt_r(x, r) ? (x + nxt_r(x, r) - 1) / nxt_r(x, r) : INF) -
           (x + r - 1) / r;
  };

  std::vector<std::multiset<int>> add(m), del(m);
  std::priority_queue<std::tuple<int, int, int>> lq, rq;
  int rp = *std::max_element(ALL(a)), lp = *std::max_element(ALL(a)) + 1;
  int rv = 0, lv = -n;
  for (int i = 0; i < n; ++i) {
    lq.emplace(nxt_l(a[i], lp), i, dif_l(a[i], lp));
    rq.emplace(nxt_r(a[i], rp), i, dif_r(a[i], rp));
  }
  auto upd = [&](int i) {
    while (lp && a[i] / lp < (a[i] + rp - 1) / rp) lp = nxt_l(a[i], lp);
    while (lp && lp <= GET(lq.top(), 0) && lv < m) {
      auto [w, i, v] = lq.top();
      lq.pop(), lv += v, lq.emplace(nxt_l(a[i], w), i, dif_l(a[i], w));
    }
  };
  auto consider = [&]() {
    add[std::max(0, rv - 1)].insert(GET(rq.top(), 0) + 1 - lp);
    if (lv < m) del[lv].insert(GET(rq.top(), 0) + 1 - lp);
    if (lv < m) minr[lv] = std::min(minr[lv], GET(rq.top(), 0) + 1);
  };

  for (int i = 0; i < n; ++i) upd(i);
  while (lp && rv <= m) {
    consider();
    auto [w, i, v] = rq.top();
    if (!w) break;
    rp = w, rv += v, rq.pop(), rq.emplace(nxt_r(a[i], w), i, dif_r(a[i], w));
    upd(i);
    while (rp <= GET(rq.top(), 0)) {
      auto [w, i, v] = rq.top();
      rq.pop(), rv += v, rq.emplace(nxt_r(a[i], w), i, dif_r(a[i], w)), upd(i);
    }
  }

  lp = *std::max_element(ALL(a)) + 1, lv = -n;
  while (lq.size()) lq.pop();
  for (int i = 0; i < n; ++i) lq.emplace(nxt_l(a[i], lp), i, dif_l(a[i], lp));
  while (lv < m) {
    auto [w, i, v] = lq.top();
    if (!w) break;
    lp = w, lv += v, lq.pop(), lq.emplace(nxt_l(a[i], w), i, dif_l(a[i], w));
    while (lp <= GET(lq.top(), 0)) {
      auto [w, i, v] = lq.top();
      lq.pop(), lv += v, lq.emplace(nxt_l(a[i], w), i, dif_l(a[i], w));
    }
    int pos = std::min(lv, m) - 1;
    if (pos >= 0) lpp[pos] = std::max(lpp[pos], lp);
  }

  for (int i = m - 1; i; --i) lpp[i - 1] = std::max(lpp[i - 1], lpp[i]);
  for (int i = 1; i < m; ++i) minr[i] = std::min(minr[i], minr[i - 1]);
  for (int i = 0; i < m; ++i)
    if (lpp[i] > 0) ans[i] = std::min(ans[i], minr[i] - lpp[i]);

  std::multiset<int> cur{INF};
  for (int i = 0; i < m; ++i) {
    for (int v : add[i]) cur.insert(v);
    for (int v : del[i]) cur.erase(cur.find(v));
    ans[i] = std::min(ans[i], *begin(cur));
  }

  return ans;
}