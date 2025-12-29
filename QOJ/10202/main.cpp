#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5;

int n, k, m;
std::array<i64, N> ans;
std::array<std::vector<std::pair<int, int>>, 30> adj;

std::array<int, N> anc;
std::array<i64, N> tmp;
void Reset() { std::iota(ALL(anc), 0); }
int Anc(int o) { return o - anc[o] ? anc[o] = Anc(anc[o]) : o; }

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> k >> m;
  for (int u, v, w; m; --m) {
    std::cin >> u >> v >> w;
    for (int e = w; e < 30; ++e) adj[e].emplace_back(u, v);
  }

  for (int e = 0; e < 30; ++e) {
    Reset(), tmp[1] = 0;
    std::vector<std::pair<int, int>> nxt;
    for (auto [x, y] : adj[e]) {
      x = Anc(x), y = Anc(y + n);
      if (x > y) std::swap(x, y);
      if (x - y) {
        anc[x] = y, ++tmp[1];
        if (x > n) nxt.emplace_back(x - n, y - n);
      }
    }
    for (int i = 2; i <= k; ++i) {
      tmp[i] = tmp[i - 1];
      std::vector<std::pair<int, int>> swp;
      for (auto [x, y] : nxt) {
        x = Anc(x), y = Anc(y);
        if (x > y) std::swap(x, y);
        if (x - y) {
          anc[x] = y;
          if (x > n) swp.emplace_back(x - n, y - n);
        } else
          --tmp[i];
      }
      nxt.swap(swp);
    }
    for (int i = 1; i <= k; ++i)
      tmp[i] += tmp[i - 1], ans[i] += i64(i + 1) * n - tmp[i];
  }

  for (int i = 1; i <= k; ++i) std::cout << ans[i] - 30 << '\n';

  return 0;
}