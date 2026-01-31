#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 3E3 + 5, MOD = 998244353;
void Add(int &x, i64 y) { x = (x + y) % MOD; }
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}
int Sum(int x, int y) { return Add(x, y), x; }
int Power(int a, int b) {
  int r = 1;
  for (; b; b >>= 1, a = i64(a) * a % MOD)
    if (b & 1) r = i64(r) * a % MOD;
  return r;
}
int Inv(int a) { return Power(a, MOD - 2); }

using P = std::vector<int>;
P operator+(P a, P b) {
  if (a.size() < b.size()) return b + a;
  for (int i = 0; i < b.size(); ++i) Add(a[i], b[i]);
  return a;
}
std::array<int, 1 << 19> omg;
void MakeO() {
  for (int i = 1; i < (1 << 19); i += i) {
    int o = Power(3, (MOD - 1) / i / 2);
    for (int j = 0, m = 1; j < i; ++j, m = i64(m) * o % MOD) omg[i ^ j] = m;
  }
}
void Dft(int *it, int n) {
  static std::array<int, 1 << 19> inv;
  static std::array<u64, 1 << 19> tmp;
  tmp[0] = it[inv[0] = 0];
  for (int i = 1; i < n; ++i)
    tmp[i] = it[inv[i] = inv[i >> 1] >> 1 | (i & 1 ? n >> 1 : 0)];
  for (int i = 1; i < n; i += i)
    for (int j = 0; j < n; j += i + i)
      for (int k = 0; k < i; ++k) {
        const u64 x = tmp[j ^ i ^ k] * omg[i ^ k] % MOD;
        tmp[j ^ i ^ k] = tmp[j ^ k] + MOD - x, tmp[j ^ k] += x;
      }
  for (int i = 0; i < n; ++i) it[i] = tmp[i] % MOD;
}
void Idft(int *it, int n) {
  std::reverse(it + 1, it + n), Dft(it, n);
  for (int i = 0, inv = Inv(n); i < n; ++i) it[i] = i64(it[i]) * inv % MOD;
}
P operator*(P a, P b) {
  if (a.empty() || b.empty()) return {};
  if (a.size() < 32 || b.size() < 32) {
    P ans(a.size() + b.size() - 1);
    for (int i = 0; i < a.size(); ++i)
      for (int j = 0; j < b.size(); ++j) Add(ans[i + j], i64(a[i]) * b[j]);
    return ans;
  }
  static std::array<int, 1 << 19> f, g;
  int n = a.size() + b.size() - 1;
  if (n ^ (n & -n)) n = 2 << std::__lg(n);
  memset(data(f), 0, n * 4), memset(data(g), 0, n * 4);
  for (int i = 0; i < a.size(); ++i) f[i] = a[i];
  for (int i = 0; i < b.size(); ++i) g[i] = b[i];
  Dft(data(f), n), Dft(data(g), n);
  for (int i = 0; i < n; ++i) f[i] = i64(f[i]) * g[i] % MOD;
  return Idft(data(f), n), std::vector(PAR(f, 0, a.size() + b.size() - 1));
}

int n;
std::array<int, N> a;

int Binom(int x, int y) {
  int ans = 1;
  for (int i = 0; i < y; ++i) ans = i64(x - i) * ans % MOD * Inv(i + 1) % MOD;
  return ans;
}
int II() {
  int s = n, ans = 0;
  for (int i = 1; i <= n; ++i) Add(s, a[i] % MOD);
  return Binom(s, n);
}
P Proc(int l, int r) {
  if (l == r) {
    P ans(n);
    for (int i = 0; i < n && i <= a[l]; ++i) ans[i] = 1;
    return ans;
  }
  int m = (l + r) >> 1;
  return Proc(l, m) * Proc(m + 1, r);
}
int A() {
  P ans = Proc(1, n);
  i64 sum = 0;
  for (int i = 1; i <= n; ++i) sum += a[i];
  int ret = 0;
  for (int i = 0; i < ans.size(); ++i) {
    if (i > sum) break;
    Add(ret, ans[i]);
  }
  return ret;
}
P Proc2(int l, int r) {
  if (l == r) {
    P ans(n);
    for (int i = 0; i < n; ++i) ans[i] = 1;
    return ans;
  }
  int m = (l + r) >> 1;
  return Proc2(l, m) * Proc2(m + 1, r);
}
int B() {
  P ans = Proc2(1, n);
  i64 sum = 0;
  for (int i = 1; i <= n; ++i) sum += a[i];
  int ret = 0;
  for (int i = 0; i < ans.size(); ++i) {
    if (i > sum) break;
    Add(ret, ans[i]);
  }
  return ret;
}
int I() { return Sum(B(), MOD - A()); }

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  MakeO();

  std::cin >> n;
  for (int i = 1; i <= n; ++i) std::cin >> a[i];
  std::cout << Sum(II(), MOD - I()) << '\n';

  return 0;
}