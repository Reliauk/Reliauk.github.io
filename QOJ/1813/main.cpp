#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 6E4 + 5;

int n;
std::array<int, N> ans;
int Median(int i, int j, int k) {
  int v;
  std::cout << "? 1 " << i << ' ' << j << ' ' << k << '\n';
  return std::cout.flush(), std::cin >> v, v;
}
int Min(int i, int j) {
  int v;
  std::cout << "? 2 " << i << ' ' << j << '\n';
  return std::cout.flush(), std::cin >> v, v;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  int a = 1, b = 2, c = 3, d = 4, l = 0, r = 0;
  std::map<int, std::map<int, int>> mp;
  int med = Median(a, b, c);
  ++mp[med][a], ++mp[med][b], ++mp[med][c];
  med = Median(a, b, d), ++mp[med][a], ++mp[med][b], ++mp[med][d];
  med = Median(a, c, d), ++mp[med][a], ++mp[med][c], ++mp[med][d];
  med = Median(b, c, d), ++mp[med][b], ++mp[med][c], ++mp[med][d];
  l = begin(mp)->first, r = rbegin(mp)->first, a = b = c = d = 0;
  for (auto [x, v] : mp[l])
    if (v == 2) (a ? b : a) = x;
  for (auto [x, v] : mp[r])
    if (v == 2) (c ? d : c) = x;

  for (int i = 5; i <= n; ++i) {
    int x = Median(i, a, c);
    int y = Median(i, b, d);
    if (x == l)
      ans[a] = l, l = y, a = i;
    else if (y == l)
      ans[b] = l, l = x, b = i;
    else if (x == r)
      ans[c] = r, r = y, c = i;
    else if (y == r)
      ans[d] = r, r = x, d = i;
    else
      ans[i] = x;
  }

  med = Min(a, b), ans[med] = 1, ans[a + b - med] = l;
  med = Min(c, d), ans[med] = r, ans[c + d - med] = n;

  std::cout << '!';
  for (int i = 1; i <= n; ++i) std::cout << ' ' << ans[i];
  std::cout << '\n', std::cout.flush();

  return 0;
}