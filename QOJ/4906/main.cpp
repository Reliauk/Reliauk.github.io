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
  auto Iter(int x) { return set.lower_bound({x, 0}); }
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
} sx, sy;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  return 0;
}