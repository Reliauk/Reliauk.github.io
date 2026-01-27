#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 205;

int n;
std::array<std::array<int, N>, N> a, pos;
std::vector<std::tuple<int, int, int, int>> ans;

void Print() {
  std::cout << ans.size() << '\n';
  for (auto [a, b, c, d] : ans)
    std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
}

void Proc() {
  std::cin >> n, ans.clear();
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j) std::cin >> a[i][j], pos[i][j] = j;

  if (n == 2) {
    if (a[1][1] == a[1][2]) ans.emplace_back(1, 1, 2, 1);
    return Print();
  }

  for (int i = 1; i <= n; ++i) {
    std::map<int, int> cnt;
    for (int j = 1; j <= n; ++j) ++cnt[a[i][j]];
    int sp = cnt.size() < 2           ? begin(cnt)->first
             : begin(cnt)->second > 1 ? rbegin(cnt)->first
                                      : begin(cnt)->first;
    for (int j = 1; j < n; ++j)
      if (a[i][j] == sp)
        std::swap(a[i][j], a[i][j + 1]), std::swap(pos[i][j], pos[i][j + 1]);
  }

  for (int i = 1; i <= n; ++i)
    for (int j = 1; j < i; ++j) ans.emplace_back(i, pos[i][j], j, pos[j][i]);

  Print();
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}