#include <bits/stdc++.h>

#include "ds.h"

#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 5E5 + 5;
std::mt19937_64 rng(20090627);

std::array<int, N> rep;
std::array<std::vector<int>, N> ch;
void Swap(int x, int y) {
  if (x - y) std::swap(rep[x], rep[y]), exchange(x, y);
}

void Proc(int l, int r) {
  std::shuffle(ALL(ch[r]), rng);
  std::vector<int> bp;
  for (int n = r - l; n; n /= 3) bp.push_back(n);
  std::reverse(ALL(bp));
  for (int n : bp) {
    std::set<std::pair<int, int>> qry{{r - n, 0}};
    for (int j = 0; j < n; ++j) {
      int i = ch[r][j], ans = query(i);
      auto it = qry.lower_bound({ans, 0});
      int val = (it == begin(qry) ? it : --it)->first + 1;
      for (; val < ans; ++val, ans = query(i)) Swap(val, ans);
      qry.emplace(std::max(ans, r - n), i);
    }
    int j = 0;
    for (auto [_, i] : qry)
      if (i) ch[r][j++] = i;
  }
}

std::array<int, N> p, q;

void solve(int n, int lim1, int lim2) {
  for (int i = 1; i < n; ++i) rep[i] = i;
  for (int i = 1; i < n; ++i) Swap(i, rng() % i + 1);
  std::vector<int> ord(n - 1);
  std::iota(ALL(ord), 2), std::shuffle(ALL(ord), rng);

  std::set<int> ed{0};
  for (int i : ord) {
    int ans = query(i);
    for (int j = *rbegin(ed) + 1; j < ans; ++j, ans = query(i)) Swap(ans, j);
    auto it = ed.lower_bound(ans);
    if (it == end(ed)) it = ed.insert(ans).first;
    ch[*it].push_back(i);
  }

  for (int r = 1, l = 0; r < n; ++r)
    if (ch[r].size()) Proc(l, r), l = r;

  std::vector<int> fa(n - 1), wf(n - 1);
  for (int i = 1, d = 0; i < n; ++i)
    for (int j = 0; j < ch[i].size(); ++j) {
      q[p[ch[i][j]] = ++d] = ch[i][j];
      if (j) fa[ch[i][j] - 2] = ch[i][j - 1];
    }
  auto swap = [&](int x, int y) {
    if (x - y) p[q[x]] = y, p[q[y]] = x, std::swap(q[x], q[y]), Swap(x, y);
  };
  for (int i = 1, d = 1; i < n; ++i) {
    if (ch[i].empty()) continue;
    swap(p[ch[i][0]], 1);
    int fz = query(ch[i][0]);
    fa[ch[i][0] - 2] = fz > 1 ? q[fz] : 1;
    for (int j : ch[i])
      if (++d < n) swap(p[j], d);
  }
  for (int i = 2; i <= n; ++i) wf[i - 2] = rep[p[i]];
  answer(fa, wf);
}