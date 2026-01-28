#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E7 + 5, INF = 1E9 + 2;

int q;
std::set<std::pair<int, int>> seg;
void Split(int x) {
  auto it = --seg.lower_bound({x + 1, 0});
  auto [l, r] = *it;
  if (l - x) seg.erase(it), seg.emplace(l, x - 1), seg.emplace(x, r);
}
struct I {
  int lc, rc;
  i64 sum;
};
int rt, d;
std::array<I, N> t;
void Add(int &o, int l, int r, int x, int v) {
  if (!o) o = ++d;
  t[o].sum += v;
  if (l == r) return;
  int m = (l + r) >> 1;
  if (x <= m)
    Add(t[o].lc, l, m, x, v);
  else
    Add(t[o].rc, m + 1, r, x, v);
}
i64 Query(int o, int l, int r, int x, int y) {
  if (x > r || l > y || !o) return 0;
  if (x <= l && r <= y) return t[o].sum;
  int m = (l + r) >> 1;
  return Query(t[o].lc, l, m, x, y) + Query(t[o].rc, m + 1, r, x, y);
}
void Reduce() {
  while (seg.size() && !Query(rt, 1, INF, 1, rbegin(seg)->first))
    seg.erase(--end(seg));
}

std::pair<bool, i64> Ask(i64 x) {
  int n = rbegin(seg)->second;
  i64 sum = 0;
  int o = rt, lo = 1, hi = INF;
  while (lo < hi) {
    i64 mi = (lo + hi) >> 1;
    if (sum + t[t[o].lc].sum + (n - mi + 1) < x)
      o = t[o].lc, hi = mi;
    else
      sum += t[t[o].lc].sum, o = t[o].rc, lo = mi + 1;
  }
  --lo, sum += (n - lo + 1);
  if (sum == x) return {true, lo};
  return {false, x - (n - lo)};
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> q;
  std::vector<std::tuple<int, int, int>> stk;
  for (int t = 1; t <= q; ++t) {
    char op;
    std::cin >> op;
    if (op == '+') {
      int a, b;
      std::cin >> a >> b, stk.emplace_back(t, a, b);
      Add(rt, 1, INF, 1, b), Add(rt, 1, INF, a + 1, -b);
      if (seg.empty())
        seg.emplace(1, a);
      else if (rbegin(seg)->second < a)
        seg.emplace(rbegin(seg)->second + 1, a);
      else
        Split(a + 1);
      Reduce();
    } else if (op == '-') {
      int a, b;
      std::cin >> a >> b;
      if (seg.empty() || rbegin(seg)->second < a) continue;
      Split(a);
      while (seg.size() && rbegin(seg)->first >= a) {
        i64 val = Query(rt, 1, INF, 1, rbegin(seg)->first);
        if (Query(rt, 1, INF, 1, rbegin(seg)->first) <= b) {
          auto [l, r] = *rbegin(seg);
          Add(rt, 1, INF, l, -val), Add(rt, 1, INF, r + 1, val);
          seg.erase(--end(seg));
        } else
          break;
      }
      if (seg.empty()) continue;
      Add(rt, 1, INF, a, -b), Add(rt, 1, INF, rbegin(seg)->second + 1, b);
    } else if (op == 'R') {
      int u;
      std::cin >> u;
      while (stk.size() && std::get<0>(stk.back()) >= t - u) {
        auto [_, a, b] = stk.back();
        Add(rt, 1, INF, 1, -b), Add(rt, 1, INF, a + 1, b), stk.pop_back();
      }
      Reduce();
    } else {
      i64 q;
      std::cin >> q;
      if (seg.empty()) {
        std::cout << "-1 -1\n";
        continue;
      }
      i64 p = rbegin(seg)->second + Query(rt, 1, INF, 1, 1) - 1;
      assert(p % q == 0);
      i64 lo = 0, hi = p / q;
      while (lo + 1 < hi) {
        i64 mi = (lo + hi) >> 1;
        if (Ask(mi * q + 1).first)
          hi = mi;
        else
          lo = mi;
      }
      std::cout << Ask(hi * q + 1).second << ' ' << Ask(lo * q + 1).second
                << '\n';
    }
  }

  return 0;
}