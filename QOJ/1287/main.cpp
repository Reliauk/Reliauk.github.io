#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using u128 = unsigned __int128;

const int MOD = 1E9 + 7;
void Add(int &x, i64 y) { x = (x + y) % MOD; }
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}
int Power(int a, i64 b) {
  int r = 1;
  for (; b; b >>= 1, a = i64(a) * a % MOD)
    if (b & 1) r = i64(r) * a % MOD;
  return r;
}

struct M {
  std::array<std::array<int, 12>, 12> a;
  M operator*(const M &o) const {
    M r;
    for (int i = 0; i < 12; ++i)
      for (int j = 0; j < 12; ++j) {
        u128 sum = 0;
        for (int k = 0; k < 12; ++k) sum += u64(a[i][k]) * o.a[k][j];
        r.a[i][j] = sum % MOD;
      }
    return r;
  }
};
std::array<M, 60> pw;
void Init() {
  pw[0].a[0][0] = pw[0].a[1][0] = 1;
  pw[0].a[0][1] = pw[0].a[1][1] = 1;
  pw[0].a[4][2] = 1;
  pw[0].a[0][3] = pw[0].a[5][3] = 1;
  pw[0].a[1][4] = pw[0].a[2][4] = 1;
  pw[0].a[3][5] = 1;
  pw[0].a[4][6] = 1;
  pw[0].a[2][7] = pw[0].a[5][7] = 1;
  pw[0].a[3][8] = 1;
  pw[0].a[4][9] = 1;
  pw[0].a[2][10] = pw[0].a[5][10] = 1;
  pw[0].a[3][11] = 1;
  for (int i = 1; i < 60; ++i) pw[i] = pw[i - 1] * pw[i - 1];
}

i64 n, m;
std::string s;

int F(int x, i64 n) {
  if (n < 0) return 0;
  if (x >= 6) return Power(4, n);
  if (x >= 2) return n ? Power(4, n - 1) : 1;
  return n == 0;
}
void Proc() {
  std::cin >> n >> s, m = n;
  while (s.size() > 3) {
    if (!m) return void(std::cout << "-1\n");
    std::string t;
    int o = s[0] == s[1] || s[2] == s[3];
    if (o) t += 195 - s[0];
    for (int i = o; i + 1 < s.size(); i += 2) {
      if (s[i] == s[i + 1]) return void(std::cout << "-1\n");
      t += s[i];
    }
    if ((s.size() ^ o) & 1) t += s.back();
    s = t, --m;
  }

  int id = [&]() {
    if (s.size() == 1) return s[0] - 'a';
    if (s.size() == 2) return (s[0] - 'a') * 2 + s[1] - 'a' + 2;
    if (s == "aab") return 6;
    if (s == "aba") return 7;
    if (s == "abb") return 8;
    if (s == "baa") return 9;
    if (s == "bab") return 10;
    if (s == "bba") return 11;
    return 12;
  }();
  if (id == 12) return void(std::cout << "-1\n");
  M cur;
  for (int i = 0; i < 12; ++i) cur.a[i].fill(0), cur.a[i][i] = 1;
  for (int i = 59; ~i; --i)
    if (m >> i & 1) cur = cur * pw[i];
  int val = cur.a[0][id], ans = 0;
  m = std::max<i64>(0, m - 2);
  for (int i = 0; i < 12; ++i) cur.a[i].fill(0), cur.a[i][i] = 1;
  for (int i = 59; ~i; --i)
    if (m >> i & 1) cur = cur * pw[i];
  for (int t = 0; t <= 4; ++t, cur = cur * pw[0])
    for (int i = 0; i < 12; ++i)
      if (cur.a[0][i] == val) Add(ans, F(i, n - m - t));
  std::cout << val << ' ' << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  Init();

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}