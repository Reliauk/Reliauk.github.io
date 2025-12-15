#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E7 + 5;

std::vector<int> pr;
std::array<int, N> mu;
std::array<bool, N> cps;
void Math() {
  mu[1] = 1;
  for (int i = 2; i < N; ++i) {
    if (!cps[i]) mu[i] = -1, pr.push_back(i);
    for (int p : pr) {
      if (i * p >= N) break;
      cps[i * p] = true, mu[i * p] = -mu[i];
      if (!(i % p)) {
        mu[i * p] = 0;
        break;
      }
    }
  }
  for (int i = 2; i < N; i += 2) mu[i] = 0;
  for (int i = 2; i < N; ++i) mu[i] += mu[i - 1];
}

int n, m;
void Proc() {
  std::cin >> n >> m;
  if (n > m) std::swap(n, m);
  if (m == 1) return void(std::cout << "1\n");

  auto s = [&](int n, int m) {
    i64 ans = 0;
    int p = std::min(n, m / 2 + 1);
    p -= ~p & 1, ans += i64(p - 1) * (p + 1) / 8;
    m -= m & 1, n -= ~n & 1;
    if (p + 2 <= n) {
      int l = (m - n + 1) / 2, r = (m - p - 1) / 2;
      ans += i64(r - l + 1) * (l + r) / 2;
    }
    return ans;
  };
  i64 ans = 0;
  for (int l = 1, r; l <= n; l = r + 1) {
    r = std::min(n / (n / l), m / (m / l));
    ans += s(n / l, m / l) * (mu[r] - mu[l - 1]);
  }
  std::cout << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  Math();

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}