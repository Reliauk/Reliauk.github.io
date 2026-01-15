#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

i64 n;
int m, sum;
std::map<i64, int> cnt;

void Proc() {
  cnt.clear(), sum = 0;
  cnt = {{1, 0}, {n + 1, 0}};
  for (int c; m; --m) {
    std::cin >> c;
    std::vector<i64> w(c + c);
    for (i64 &v : w) std::cin >> v;
    std::string tok;
    std::cin >> tok;
    int val = tok == "yes" ? 1 : -1;
    sum += std::max(val, 0);
    for (int i = 0; i < c + c; i += 2)
      cnt[w[i]] += val, cnt[w[i + 1] + 1] -= val;
  }
  i64 f = 0, g = 0;
  for (auto it = begin(cnt); it->first <= n; ++it) {
    sum -= it->second;
    i64 len = next(it)->first - it->first;
    if (sum == 0) f += len;
    if (sum == 1) g += len;
  }
  i64 q = 0;
  while ((q + 1) * f + (f & 1 ? std::max(q - 1, g) : g) > (i64(1) << q)) ++q;
  std::cout << q << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  while (std::cin >> n >> m) Proc();

  return 0;
}