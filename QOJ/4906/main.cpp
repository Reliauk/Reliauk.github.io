#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E6 + 5, INF = 2E9;

int n;
std::array<int, N> t, x;

struct S {
  int dt;
  std::set<std::pair<int, int>> set;
  auto Iter(int x) { return set.lower_bound({x, -INF}); }
  void Push(int l, int r) {
    l += dt, r -= dt;
    auto it = Iter(l + 1);
    if (it != begin(set) && prev(it)->second >= r) return;
    it = next(set.emplace(l, r).first);
    while (it != end(set) && it->second <= r) set.erase(it++);
  }
  int Dis(int x) {
    auto it = Iter(x + dt + 1);
    if (it != begin(set) && prev(it)->second + dt >= x) return 0;
    int ans = INF;
    if (it != begin(set)) ans = std::min(ans, x - prev(it)->second - dt);
    if (it != end(set)) ans = std::min(ans, it->first - dt - x);
    return ans;
  }
  int Next(int x) {
    auto it = Iter(x + dt + 1);
    if (it != begin(set) && prev(it)->second + dt >= x) return x;
    return it == end(set) ? INF : it->first - dt;
  }
  int Prev(int x) {
    auto it = Iter(x + dt + 1);
    if (it != begin(set) && prev(it)->second + dt >= x) return x;
    return it == begin(set) ? -INF : prev(it)->second + dt;
  }
} sx, sy;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  for (int i = 1; i <= n; ++i) std::cin >> t[i] >> x[i];

  sx.Push(0, 0), sy.Push(0, 0);
  for (int i = 1; i <= n; ++i) {
    std::vector<std::pair<int, int>> lz;
    int tm = t[i] - t[i - 1], ds = std::abs(x[i] - x[i - 1]);
    if (!sx.Dis(x[i])) lz.emplace_back(x[i - 1] - tm, x[i - 1] + tm);
    if (ds <= tm && sx.set.size()) {
      lz.emplace_back(x[i] - tm + ds, x[i] + tm - ds);
      int mn = std::min(x[i - 1], x[i]), mx = std::max(x[i - 1], x[i]);
      sx.Push(mn - (tm - ds) / 2, mx + (tm - ds) / 2);
    } else
      sx.set.clear();
    int lp = sy.Next(x[i] - tm), rp = sy.Prev(x[i] + tm);
    if (lp <= rp) sx.Push((lp + x[i] - tm + 1) >> 1, (rp + x[i] + tm) >> 1);
    if (int md = sy.Dis(x[i]); md <= tm) {
      sx.Push(x[i - 1], x[i - 1]);
      lz.emplace_back(x[i] - tm + md, x[i] + tm - md);
    }
    if (x[i - 1] == x[i])
      sy.dt += tm;
    else
      sy.set.clear();
    for (auto [l, r] : lz) sy.Push(l, r);
    if (sx.set.empty() && sy.set.empty()) std::cout << "NO\n", exit(0);
  }

  std::cout << "YES\n";

  return 0;
}