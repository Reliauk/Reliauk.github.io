#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5;

int n, q;
std::array<int, N> a;

i64 sum;
std::set<int> suf;
std::array<int, N> c;
void Proc(int x, int y, int o) {
  if (~x & 1) sum += o * (a[x] - a[y]), c[x] += o, c[y] -= o;
}
void Push(int x) {
  auto it = suf.insert(x).first;
  if (it != begin(suf) && next(it) != end(suf)) Proc(*prev(it), *next(it), -1);
  if (it != begin(suf)) Proc(*prev(it), *it, 1);
  if (next(it) != end(suf)) Proc(*it, *next(it), 1);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  for (int i = 1; i <= n; ++i) std::cin >> a[i];
  sum += a[n];
  for (int i = 1; i < n; i += 2) sum += a[i];
  for (int i = n, g = INT_MAX; i; --i)
    if (a[i] < g) Push(i), g = a[i];

  std::cout << sum << '\n', std::cin >> q;
  for (int x, v; q; --q) {
    std::cin >> x >> v, a[x] -= v, sum -= c[x] * v;
    if ((x & 1) || x == n) sum -= v;
    int g = x == n ? INT_MAX : a[*suf.upper_bound(x)];
    if (a[x] < g) {
      if (!suf.count(x)) Push(x);
      for (auto it = suf.find(x); it != begin(suf) && a[*prev(it)] >= a[x];) {
        if (prev(it) != begin(suf))
          Proc(*prev(prev(it)), *prev(it), -1), Proc(*prev(prev(it)), x, 1);
        Proc(*prev(it), x, -1), suf.erase(prev(it));
      }
    }
    std::cout << sum << '\n';
  }

  return 0;
}