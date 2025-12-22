#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5;

int n, m;
i64 ans;
std::string s;
std::array<int, N> a, b;

std::set<int> ind;
std::array<int, N> typ;
std::array<std::priority_queue<int>, N> pq;
void Merge(int c, int d) {
  if (pq[c].size() > pq[d].size()) std::swap(pq[c], pq[d]);
  while (pq[c].size()) pq[d].push(pq[c].top() * typ[c] * typ[d]), pq[c].pop();
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  for (int i = 1; i <= n; ++i) std::cin >> a[i];
  std::cin >> m;
  for (int i = 1; i <= m; ++i) std::cin >> b[i];
  std::cin >> s, s = ' ' + s;

  for (int l = 1, r, c = 1; l <= m; l = r, ++c) {
    ind.insert(c), typ[c] = 1 - 2 * (s[l] == 'S');
    for (r = l; r <= m && s[r] == s[l]; ++r) pq[c].push(typ[c] * b[r]);
  }

  for (int i = 1; i <= n; ++i) {
    a[i] = (a[i] + ans) % int(1E9);
    for (int c : ind)
      pq[c].push(typ[c] * a[i]), a[i] = typ[c] * pq[c].top(), pq[c].pop();
    for (auto it = begin(ind); next(it) != end(ind);) {
      if (next(next(it)) == end(ind)) break;
      int c = *it, d = *next(it);
      bool swp = typ[c] > 0 && pq[c].top() * typ[c] <= pq[d].top() * typ[d];
      swp |= typ[c] < 0 && pq[c].top() * typ[c] >= pq[d].top() * typ[d];
      if (swp) {
        Merge(c, *next(next(it)));
        if (it != begin(ind))
          Merge(d, *prev(it)), ind.erase(it++), ind.erase(it++);
        else
          ind.erase(it++), ++it;
      } else
        ++it;
    }
    std::cout << (ans += a[i]) << "\n "[i < n];
  }

  return 0;
}