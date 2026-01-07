#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

int n, m;
i64 ans;
std::vector<int> p, e;

i64 S(i64 n) { return n * (n + 1) / 2; }
i64 Euc(i64 a, i64 b, i64 c, i64 n) {
  if (!n) return 0;
  if (a < 0) return Euc(-a, b + (n + 1) * a, c, n);
  if (a >= c) return a / c * S(n) + Euc(a % c, b, c, n);
  if (b >= c) return b / c * n + Euc(a, b % c, c, n);
  if (b < 0) {
    i64 x = (c - 1 - b) / c;
    return Euc(a, b + c * x, c, n) - x * n;
  }
  i64 v = (a * n + b) / c;
  return n * v - Euc(c, -b - 1, a, v);
}
i64 Eval(int d, int t) {
  auto f = [&](i64 a, int b, int c) {
    return b > a ? 0 : Euc(-b, a, c, a / b);
  };
  if (d * t % 2 && m / d % 2)
    return f(n + n - 2, d * t * 2, m / d * 2) +
           f(i64(n) + n - 2 + d * t + m / d, d * t * 2, m / d * 2);
  if (d * t % 2) return f(n + n - 2, d * t * 2, m / d);
  if (m / d % 2) return f(n + n - 2, d * t, m / d * 2);
  return f(n + n - 2, d * t, m / d);
}
void Dfs(int o, int d, int t, int mu) {
  if (o == p.size()) return void(ans += mu * Eval(d, t));
  for (int i = 0; i <= e[o]; ++i, d *= p[o])
    if (Dfs(o + 1, d, t, mu); i < e[o]) Dfs(o + 1, d, t * p[o], -mu);
}

void Proc() {
  std::cin >> n >> m, p = e = {};
  int k = m;
  for (int i = 2; i * i <= k; ++i) {
    if (k % i) continue;
    p.push_back(i), e.push_back(0);
    while (!(k % i)) k /= i, ++e.back();
  }
  if (k > 1) p.push_back(k), e.push_back(1);
  ans = 0, Dfs(0, 1, 1, 1);
  int min = 1;
  for (int i = 0; i < p.size(); ++i)
    for (int j = 0; j < (e[i] + 1) / 2; ++j) min *= p[i];
  std::cout << (ans + (n - 1) / min) / 2 << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}