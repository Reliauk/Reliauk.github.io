#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using i128 = __int128;
void Print(i128 x) {
  if (x >= 10) Print(x / 10);
  std::cout << int(x % 10);
}

const int MOD = 998244353;
using PL = std::vector<int>;

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
template <typename T>
void Dft(T it, int n) {
  std::vector inv(n, 0);
  std::vector<u64> t(n);
  t[0] = it[0];
  for (int i = 1; i < n; ++i) {
    inv[i] = inv[i >> 1] >> 1 | (i & 1 ? n >> 1 : 0);
    t[i] = it[inv[i]];
  }
  for (int i = 1; i < n; i += i) {
    if (i >> 19 & 1)
      for (int k = 0; k < n; ++k) t[k] %= MOD;
    int o = Power(3, (MOD - 1) / i / 2);
    for (int j = 0; j < n; j += i + i)
      for (int k = 0, m = 1; k < i; ++k, m = i64(m) * o % MOD) {
        const u64 x = t[j + k + i] * m % MOD;
        t[j + k + i] = t[j + k] + MOD - x, t[j + k] += x;
      }
  }
  for (int k = 0; k < n; ++k) it[k] = t[k] % MOD;
}
template <typename T>
void Idft(T it, int n) {
  std::reverse(it + 1, it + n), Dft(it, n);
  for (int i = 0, inv = Inv(n); i < n; ++i) it[i] = i64(it[i]) * inv % MOD;
}
PL operator*(PL a, PL b) {
  int ol = a.size() + b.size() - 1, l = 1 << (std::__lg(ol) + 1);
  a.resize(l), b.resize(l), Dft(begin(a), l), Dft(begin(b), l);
  for (int i = 0; i < l; ++i) a[i] = i64(a[i]) * b[i] % MOD;
  Idft(begin(a), l), a.resize(ol);
  return a;
}

struct P {
  std::vector<int> v;
  P() = default;
  P(i128 n) {
    while (n) v.push_back(n % 10), n /= 10;
  }
  void Carry() {
    i64 c = 0;
    for (int i = 0; i < v.size(); ++i) c += v[i], v[i] = c % 10, c /= 10;
    while (c) v.push_back(c % 10), c /= 10;
  }
  P operator+(const P &o) const {
    P r;
    r.v.resize(std::max(v.size(), o.v.size()));
    for (int i = 0; i < v.size(); ++i) r.v[i] = v[i];
    for (int i = 0; i < o.v.size(); ++i) r.v[i] += o.v[i];
    return r.Carry(), r;
  }
  P operator-(const P &o) const {
    P r = *this;
    for (int i = 0; i < o.v.size(); ++i) {
      r.v[i] -= o.v[i];
      if (r.v[i] < 0) r.v[i] += 10, --r.v[i + 1];
    }
    return r;
  }
  P operator*(const P &o) const {
    P r;
    if (v.empty() || o.v.empty()) return r;
    if (v.size() < 128 || o.v.size() < 128) {
      r.v.resize(v.size() + o.v.size() - 1);
      for (int i = 0; i < v.size(); ++i)
        for (int j = 0; j < o.v.size(); ++j) r.v[i + j] += v[i] * o.v[j];
    } else
      r.v = v * o.v;
    return r.Carry(), r;
  }
  i64 operator%(i64 x) {
    i64 r = 0;
    for (int i = int(v.size()) - 1; ~i; --i) r = (i128(r) * 10 + v[i]) % x;
    return r;
  }
  P operator/(i64 x) {
    P ans;
    i128 r = 0;
    for (int i = int(v.size()) - 1; ~i; --i)
      r = r * 10 + v[i], ans.v.push_back(r / x), r %= x;
    reverse(ALL(ans.v));
    while (ans.v.size() && !ans.v.back()) ans.v.pop_back();
    return ans;
  }
  void Print() {
    if (v.empty()) v.push_back(0);
    for (int i = v.size() - 1; ~i; --i) std::cout << v[i];
  }
};

const int N = 8E5 + 5;

int n, m, b, l, r;
i64 sl, sr;

std::vector<int> prm;
std::array<int, N> cnt;
std::array<bool, N> cps;
int Nu(int n, int p) {
  int ans = 0;
  while (n /= p) ans += n;
  return ans;
}

P num, den;
std::vector<P> vn, vd;
template <typename T>
P Mul(T bg, T ed) {
  if (bg == ed) return P(1);
  if (bg + 1 == ed) return *bg;
  T md = bg + (ed - bg) / 2;
  return Mul(bg, md) * Mul(md, ed);
}
void Div() {
  for (int i = 2; i <= n; ++i) {
    if (!cps[i]) prm.push_back(i);
    for (int p : prm) {
      if (i * p > n) break;
      cps[i * p] = true;
      if (!(i % p)) break;
    }
  }
  for (int p : prm) {
    cnt[p] = Nu(r, p) + Nu(n - m - 1, p) - Nu(n, p) - Nu(r - m - 1, p);
    if (cnt[p] > 0)
      for (int i = 0; i < cnt[p]; ++i) vn.push_back(P(p));
    else if (cnt[p] < 0)
      for (int i = 0; i < -cnt[p]; ++i) vd.push_back(P(p));
  }
  num = Mul(ALL(vn)), den = Mul(ALL(vd));
}
void Mul(i64 x) {
  i64 g = std::gcd(x, den % x);
  den = den / g, num = num * P(x / g);
}
void Div(i64 x) {
  i64 g = std::gcd(x, num % x);
  num = num / g, den = den * P(x / g);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m >> b;
  for (int i = 0, a; i < n; ++i)
    std::cin >> a, ++(a > b ? r : l), (a > b ? sr : sl) += a;

  if (!l || !r) {
    i64 x = (sl + sr) * m, y = n, g = std::gcd(x, y);
    std::cout << x / g << '\n' << y / g << '\n', exit(0);
  }
  if (n == m) {
    i128 x = i128(sr) * (n - r + 1) + i128(n + 1) * sl * (l + 1);
    i128 y = i128(n - r + 1) * (l + 1), g = std::__gcd(x, y);
    Print(x / g), std::cout << '\n', Print(y / g), std::cout << '\n', exit(0);
  }

  if (r > m)
    Div();
  else
    den = {1};
  Mul(sr * l - sl * r), Mul(n - m);
  i128 x = i128(sr * l - sl * r) * r - i128(m) * r * sl * (r - n - 1);
  assert(x >= 0), num = P(x) * den - num;
  Div(l), Div(r), Div(n + 1 - r);
  num.Print(), std::cout << '\n', den.Print(), std::cout << '\n';

  return 0;
}