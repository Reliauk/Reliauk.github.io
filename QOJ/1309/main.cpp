#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using i128 = __int128;

const int N = 55, MOD = 998244353;
void Add(int &x, i64 y) { x = (x + y) % MOD; }
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}
int S(int n) { return i64(n) * (n + 1) / 2 % MOD; }
int S2(int n) { return i128(n) * (n + 1) * (n + n + 1) / 6 % MOD; }

int n, k;
std::array<int, N> x, y;

std::vector<int> raw;
std::array<std::map<int, int>, 3> dif;
void Push(int a, int b, int w) {
  int sep = std::max(a, b) >> 1;
  Add(dif[0][sep], i64(a + MOD - 1) * (b + MOD - 1) % MOD * w);
  Add(dif[1][sep], i64(MOD + 2 - a - b) * 2 * w);
  Add(dif[2][sep], i64(4) * w), raw.push_back(sep);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> k;
  for (int i = 1; i <= n; ++i) std::cin >> x[i] >> y[i];
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j < n; ++j)
      if (x[j] > x[j + 1]) std::swap(x[j], x[j + 1]), std::swap(y[j], y[j + 1]);

  raw = {0}, Push(0, 0, n);
  for (int l = 1; l <= n; ++l) {
    std::vector<int> ind;
    for (int r = l + 1; r <= n; ++r) {
      auto [mn, mx] = std::minmax(y[l], y[r]);
      Push(x[r] - x[l], mx - mn, MOD - 1);
      for (int i : ind)
        Push(x[r] - x[l], std::max(y[i], mx) - std::min(y[i], mn), 1);
      for (int i = 1; i < ind.size(); ++i) {
        int lp = std::min(y[ind[i - 1]], mn), rp = std::max(y[ind[i]], mx);
        Push(x[r] - x[l], rp - lp, MOD - 1);
      }
      int p = 0;
      while (p < ind.size() && y[ind[p]] < y[r]) ++p;
      ind.insert(begin(ind) + p, r);
    }
  }

  sort(ALL(raw)), raw.erase(unique(ALL(raw)), end(raw));
  int c0 = 0, c1 = 0, c2 = 0, s = 0, f = 0, sf = 0;
  int itf = upper_bound(ALL(raw), k - 1) - begin(raw) - 1;
  int itsf = upper_bound(ALL(raw), k - 2) - begin(raw) - 1;
  for (int i = 0; i < raw.size(); ++i) {
    int x = raw[i];
    Add(c0, dif[0][x]), Add(c1, dif[1][x]), Add(c2, dif[2][x]);
    if (i == itf)
      f = (i64(c2) * (k - 1) % MOD * (k - 1) + i64(c1) * (k - 1) + c0) % MOD;
    if (i == itsf) {
      sf = s;
      Add(sf, i64(k - 2 - x + 1) * c0);
      Add(sf, i64(S(k - 2) - S(x - 1) + MOD) * c1);
      Add(sf, i64(S2(k - 2) - S2(x - 1) + MOD) * c2);
    }
    if (i + 1 < raw.size()) {
      int y = raw[i + 1] - 1;
      Add(s, i64(y - x + 1) * c0);
      Add(s, i64(S(y) - S(x - 1) + MOD) * c1);
      Add(s, i64(S2(y) - S2(x - 1) + MOD) * c2);
    }
  }

  std::cout << (i64(f) * k + MOD - sf) % MOD << '\n';

  return 0;
}