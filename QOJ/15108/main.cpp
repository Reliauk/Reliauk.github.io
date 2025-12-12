#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 3E5 + 5;
const i64 INF = 1E18;

int n, m;
std::array<i64, N> a, c, s, ans;

i64 tg;
struct S {
  int l, r;
  i64 c;
  S() = default;
  S(int l_, int r_, i64 c_) { l = l_, r = r_, c = c_; }
  bool operator<(const S &o) const { return l < o.l; }
};
std::set<S> set;
void Split(int x) {
  if (x > m) return;
  auto it = --set.upper_bound(S(x, 0, 0));
  if (it->l == x) return;
  auto [l, r, c] = *it;
  set.erase(it), set.emplace(l, x - 1, c), set.emplace(x, r, c);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int i = 1; i <= n; ++i) std::cin >> a[i];
  for (int i = 1; i <= m; ++i) std::cin >> c[i], s[i] = c[i] + s[i - 1];

  std::priority_queue<i64> pq;
  pq.push(-INF), set = {{1, m, 0}};
  for (int i = n; i; --i) {
    ++tg;
    for (i64 x; pq.size() && a[i] >= 0; a[i] -= x, ++ans[i], pq.pop()) {
      int p = std::upper_bound(PAR(c, 1, m + 1), x = -pq.top()) - data(c);
      Split(p);
      auto it = begin(set);
      while (it != end(set) && it->r < p) {
        auto [l, r, c] = *it;
        if ((c + tg) * (s[r] - s[l - 1]) <= a[i]) {
          a[i] -= (c + tg) * (s[r] - s[l - 1]);
          ans[i] += (c + tg) * (r - l + 1), set.erase(it++);
        } else
          break;
      }
      if (it == end(set) || it->l == p) continue;
      auto [l, r, v] = *it;
      int lo = l, hi = r;
      while (lo < hi) {
        int mi = (lo + hi) >> 1;
        if ((s[mi] - s[l - 1]) * (v + tg) > a[i])
          hi = mi;
        else
          lo = mi + 1;
      }
      a[i] -= (v + tg) * (s[lo - 1] - s[l - 1]);
      ans[i] += (v + tg) * (lo - l), l = lo;
      x = a[i] / c[l] + 1, ans[i] += x, a[i] -= x * c[l];
      set.erase(it), set.emplace(l, l, v - x);
      if (l > 1) set.emplace(1, l - 1, -tg);
      if (l < r) set.emplace(l + 1, r, v);
      break;
    }
    if (set.empty() || begin(set)->l != 1) {
      int rp = set.size() ? begin(set)->l - 1 : m;
      set.emplace(1, rp, -tg);
    }
    if (a[i] < 0) pq.push(a[i]), --ans[i];
  }

  for (int i = n; i > 1; --i) ans[i - 1] += ans[i];
  for (int i = 1; i <= n; ++i) std::cout << ans[i] << "\n "[i < n];

  return 0;
}