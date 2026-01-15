#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int MOD = 1E9 + 7;
void Add(int &x, i64 y) { x = (x + y) % MOD; }
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}

int s, n, m, ans;
std::array<int, 1 << 12> coef;
std::array<int, 1 << 24> dp;
std::array<std::bitset<1 << 12>, 1 << 12> eq, le, ge, ne, lt, gt;

void Proc() {
  for (int s = 0; s < 1 << (n + n); ++s) dp[s] = 0;
  for (int s = 0; s < 1 << n; ++s) {
    eq[s].reset(), le[s].reset(), ge[s].reset();
    ne[s].reset(), lt[s].reset(), gt[s].reset();
  }
  for (int x, y; m; --m) {
    std::string op;
    std::cin >> x >> op >> y, --x, --y;
    std::bitset<1 << 12> cur;
    int sta = 0;
    for (int s = 0; s < (1 << n); ++s)
      if (s >> y & 1) cur[s] = true;
    (op == "="    ? eq
     : op == "<=" ? le
     : op == ">=" ? ge
     : op == "!=" ? ne
     : op == "<"  ? lt
                  : gt)[1 << x] |= cur;
  }
  for (int i = 0; i < n; ++i)
    for (int s = 0; s < (1 << n); ++s)
      if (s >> i & 1) {
        eq[s] |= eq[s ^ 1 << i], le[s] |= le[s ^ 1 << i];
        ge[s] |= ge[s ^ 1 << i], ne[s] |= ne[s ^ 1 << i];
        lt[s] |= lt[s ^ 1 << i], gt[s] |= gt[s ^ 1 << i];
      }
  auto valid = [&](int s, int t) {
    int co = (1 << n) - 1 - t;
    if (eq[t][co]) return false;
    if (le[t][s]) return false;
    if (ge[s][t]) return false;
    if (ne[t][t]) return false;
    if (lt[t][t ^ s]) return false;
    return !gt[s ^ t][t];
  };
  for (int s = 0; s < 1 << n; ++s) coef[s] = 0;
  for (int s = 0; s < 1 << (n + n); ++s) {
    int sta = s >> n, cut = s ^ (sta << n);
    if (!cut || std::__lg(cut) + 1 - __builtin_popcount(sta)) continue;
    if (valid(0, sta) && (cut == (cut & -cut))) Add(dp[s], 1);
    if (!dp[s]) continue;
    int u = (1 << n) - 1 - sta;
    if (!u) Add(coef[cut], dp[s]);
    for (int t = u; t; --t &= u) {
      int cnt = __builtin_popcount(sta ^ t) - 1;
      if (valid(sta, t)) Add(dp[(sta ^ t) << n ^ cut ^ (1 << cnt)], dp[s]);
    }
  }

  ans = 0;
  for (int s = 1 << (n - 1); s < 1 << n; ++s) {
    int v = ::s - n;
    for (int i = 0; i < n; ++i)
      if (s >> i & 1) v -= n - i - 1;
    if (v < 0 || !coef[s]) continue;
    int sum = ::s - v;
    std::vector dp(sum + 1, 0), nxt(sum * 2 + 1, 0);
    dp[0] = 1;
    for (int d = 0; d < 30; ++d) {
      for (int i = 0; i <= sum; ++i) nxt[i] = dp[i], dp[i] = 0;
      for (int i = 1; i <= n; ++i) {
        if (i < n && (~s >> (n - i - 1) & 1)) continue;
        for (int j = sum * 2; j >= i; --j) Add(nxt[j], nxt[j - i]);
      }
      for (int i = 0; i <= sum * 2; ++i) {
        if ((i & 1) == (v >> d & 1)) dp[i >> 1] = nxt[i];
        nxt[i] = 0;
      }
    }
    Add(ans, i64(dp[0]) * coef[s]);
  }
  std::cout << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  while (std::cin >> s >> n >> m) Proc();

  return 0;
}