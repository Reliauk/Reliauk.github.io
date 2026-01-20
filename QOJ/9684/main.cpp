#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E4 + 5, A = 22, MOD = 1E9 + 7;
void Add(int &x, i64 y) { x = (x + y) % MOD; }
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}
int Sum(int x, int y) { return Add(x, y), x; }
std::array<int, N * 2> pw, ipw;
void Math() {
  for (int i = pw[0] = 1; i < N * 2; ++i) Add(pw[i] = pw[i - 1], pw[i - 1]);
  for (int i = ipw[0] = 1; i < N * 2; ++i)
    ipw[i] = i64(MOD + 1) / 2 * ipw[i - 1] % MOD;
}
std::mt19937_64 rng(20090627);

int n, k;
std::array<int, N> a;

std::array<std::array<int, A>, N> lp, rp, mp;

std::array<int, N> hash, g;
std::array<std::array<int, A>, N> f;
int F(int l, int r) {
  if (r - l < A) return f[r][r - l];
  return f[r][A - 1] + (l <= g[r]);
}
int Hash(int l, int r, int t) {
  int ans = 0;
  if (t <= r) {
    int sum = Sum(hash[r], MOD - hash[std::max(l, t) - 1]);
    Add(ans, i64(t < 0 ? pw[-t] : ipw[t]) * sum);
  }
  if (l < t) {
    int sum = F(l, std::min(t - 1, r));
    if (t > r) sum = (t - r - 1 <= 30 ? sum >> (t - r - 1) : 0);
    Add(ans, sum);
  }
  return ans;
}
bool Leq(int al, int ar, int at, int bl, int br, int bt) {
  int hi = -A;
  for (int d = 0; hi < n; ++d) {
    hi = std::min(n, hi + (1 << d));
    if (Hash(al, ar, at - hi) - Hash(bl, br, bt - hi)) break;
  }
  int lo = -A;
  while (lo < hi) {
    int mi = (lo + hi + 1) >> 1;
    bool chk = Hash(al, ar, at - mi) == Hash(bl, br, bt - mi);
    (chk ? lo : hi) = mi - !chk;
  }
  int dif = Sum(Hash(al, ar, at - lo - 1), MOD - Hash(bl, br, bt - lo - 1));
  if (dif) return dif != 1;
  return std::tuple{al, ar, at} <= std::tuple{bl, br, bt};
}

std::array<int, N> dp, bit;
void Cmin(int x, int v) {
  for (++x; x; x &= x - 1) bit[x] = std::min(bit[x], v);
}
int Query(int x) {
  int v = MOD;
  for (++x; x <= n + 1; x += x & -x) v = std::min(v, bit[x]);
  return v;
}
bool Check(int ml, int mr, int mt) {
  for (int r = 1; r <= n; ++r) dp[r] = MOD;
  for (int i = 1; i <= n + 1; ++i) bit[i] = MOD;
  Cmin(0, -1);
  for (int t = 1; t <= n; ++t) {
    for (int r = std::max(1, t - A + 1); r <= t; ++r)
      dp[r] = std::min(dp[r], t + Query(mp[r][t - r] - 1));
    for (int r = std::max(1, t - A + 1); r <= t; ++r) Cmin(r, dp[r] - r - 1);
  }
  return dp[n] <= k;
}

void Proc() {
  std::cin >> n >> k;
  for (int i = 1; i <= n; ++i)
    std::cin >> a[i], Add(hash[i] = hash[i - 1], i64(a[i]) * pw[i]);

  for (int r = 1; r <= n; ++r)
    for (int t = 0; t + r <= n && t < A; ++t) lp[r][t] = 1, rp[r][t] = r;

  for (int r = 1; r <= n; ++r)
    for (int l = std::max(r - A + 1, 1); l <= r; ++l)
      f[r][r - l] = (l == r ? a[l] : f[r - 1][r - 1 - l] + a[r]) >> 1;
  if (A + 1 <= n) {
    int val = (f[A][A - 1] + a[A + 1]) >> 1;
    g[A + 1] = (val != f[A + 1][A - 1]);
    for (int i = A + 2; i <= n; ++i) {
      int v0 = f[i - 1][A - 1], v1 = v0 + 1;
      if (((v0 + a[i]) >> 1) != ((v1 + a[i]) >> 1))
        g[i] = g[i - 1];
      else
        g[i] = 0;
      int val = (f[i - 1][A - 1] + a[i]) >> 1;
      if (val != f[i][A - 1]) g[i] = i - A;
    }
  }

  while (true) {
    int siz = 0;
    for (int r = 1; r <= n; ++r)
      for (int t = 0; t + r <= n && t < A; ++t)
        siz += std::max(rp[r][t] - lp[r][t] + 1, 0);
    if (siz == 1) break;

    int ml = 0, mr = 0, mt = 0;
    int mid = std::uniform_int_distribution<int>(1, siz)(rng);
    for (int r = 1; r <= n; ++r)
      for (int t = 0; t + r <= n && t < A; ++t) {
        int cnt = std::max(rp[r][t] - lp[r][t] + 1, 0);
        if (mid > 0 && mid <= cnt) ml = lp[r][t] + mid - 1, mr = r, mt = t;
        mid -= cnt;
      }

    for (int r = 1; r <= n; ++r)
      for (int t = 0; t + r <= n && t < A; ++t) {
        int lo = lp[r][t], hi = rp[r][t] + 1;
        while (lo < hi) {
          int mi = (lo + hi) >> 1;
          bool chk = Leq(mi, r, r + t, ml, mr, mr + mt);
          (chk ? hi : lo) = mi + !chk;
        }
        mp[r][t] = lo;
      }

    bool chk = Check(ml, mr, mr + mt);
    for (int r = 1; r <= n; ++r)
      for (int t = 0; t + r <= n && t < A; ++t)
        (chk ? lp[r][t] : rp[r][t]) = mp[r][t] - !chk;
  }

  int al = 0, ar = 0, at = 0;
  for (int r = 1; r <= n; ++r)
    for (int t = 0; t + r <= n && t < A; ++t)
      if (lp[r][t] <= rp[r][t]) al = lp[r][t], ar = r, at = t;
  std::string ans;
  for (int x = -A; x <= n; ++x) {
    int lst = Hash(al, ar, ar + at - x + 1);
    int cur = Hash(al, ar, ar + at - x);
    ans += '0' + (i64(MOD - lst) * 2 + cur) % MOD;
  }
  reverse(ALL(ans));
  while (ans.size() > 1 && ans.back() == '0') ans.pop_back();
  reverse(ALL(ans)), std::cout << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t, Math();
  while (t--) Proc();

  return 0;
}