#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E6 + 5;

int m;
std::array<std::vector<int>, N> z, o;
int F(int x, int i) {
  if (i < 0) return x == m ? m : m + 1;
  int cnt = 0, ans = 0;
  if (x - z[m][i] <= m) ans = x - z[m][i], ++cnt;
  if (o[m][i] && o[m][i] <= m && z[m][i] - 1 + o[m][i] == x)
    ans = o[m][i], ++cnt;
  if (cnt > 1) return m + 1;
  if ((x - z[m][i]) % 2 == 0)
    if (F((x + z[m][i]) / 2, i) <= m) ans = (x - z[m][i]) / 2, ++cnt;
  if (cnt > 1) return m + 1;
  int ed = z[m][i] - 1 - !!o[m][i];
  if (x <= ed + m && (ed + m - x) % 2 == 0)
    if (F((ed - m + x) / 2, i - 1) <= m) ans = (x + m - ed) / 2, ++cnt;
  return cnt > 1 ? m + 1 : cnt ? ans : 0;
}
void Prep() {
  for (m = 3; m < N; m += 2) {
    z[m].push_back(m + 1), o[m].push_back(0);
    while (z[m].back() + m + 2 < N) {
      int x = z[m].back() + m + 1, f = F(x, z[m].size() - 1);
      z[m].push_back(x + !!f), o[m].push_back(f);
    }
  }
}

void Proc() {
  int n, m;
  std::cin >> n >> m;
  if (~m & 1) return void(std::cout << (n % (m + 1) ? "YES\n" : "NO\n"));
  auto it = lower_bound(ALL(z[m]), n);
  std::cout << (it != end(z[m]) && *it == n ? "NO\n" : "YES\n");
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t, Prep();
  while (t--) Proc();

  return 0;
}