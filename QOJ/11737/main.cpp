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

struct P {
  std::vector<int> v;
  P() = default;
  P(int n) {
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
  P operator*(const P &o) const {
    P r;
    if (v.empty() || o.v.empty()) return r;
    r.v.resize(v.size() + o.v.size() - 1);
    for (int i = 0; i < v.size(); ++i)
      for (int j = 0; j < o.v.size(); ++j) r.v[i + j] += v[i] * o.v[j];
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
      if (i * p > n) continue;
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

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m >> b;
  for (int i = 0, a; i < n; ++i)
    std::cin >> a, ++(a > b ? r : l), (a > b ? sr : sl) += a;

  if (n == m) {
    i128 x = 2 * sr + i128(n + n - r) * sl * (l + 1);
    i128 y = 2 * (l + 1), g = std::gcd(x, y);
    Print(x / g), std::cout << '\n', Print(y / g), std::cout << '\n', exit(0);
  }
  if (!l || !r) {
    i64 x = (sl + sr) * m, y = n, g = std::gcd(x, y);
    std::cout << x / g << '\n' << y / g << '\n', exit(0);
  }

  if (r > m)
    Div();
  else
    vd = {1};
  Mul()

      return 0;
}