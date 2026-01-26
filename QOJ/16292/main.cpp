#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5;

int n;
i64 ans;
std::array<int, N> x, y;

std::vector<int> pos, neg;
std::array<int, N> lp, rp;

void Proc() {
  std::cin >> n, ans = 0;
  for (int i = 1, h, p, s; i <= n; ++i)
    std::cin >> h >> p >> s, x[i] = h - p, y[i] = s - p;

  pos.clear(), neg.clear();
  for (int i = 1; i <= n; ++i) {
    if (y[i] > 0) pos.push_back(i);
    if (y[i] < 0) neg.push_back(i);
  }
  auto comp = [&](int i, int j) { return i64(x[i]) * y[j] < i64(y[i]) * x[j]; };
  std::sort(ALL(pos), comp), std::sort(ALL(neg), comp);

  int ptr = 0;
  i64 pgn = 0, ngp = 0;
  for (int i : pos) {
    while (ptr < neg.size() && comp(i, neg[ptr])) ++ptr;
    pgn += ptr, lp[i] = ptr;
  }
  ptr = 0;
  for (int i : neg) {
    while (ptr < pos.size() && comp(i, pos[ptr])) ++ptr;
    ngp += ptr, lp[i] = ptr;
  }
  ptr = 0, reverse(ALL(pos)), reverse(ALL(neg));
  for (int i : pos) {
    while (ptr < neg.size() && comp(neg[ptr], i)) ++ptr;
    rp[i] = ptr;
  }
  ptr = 0;
  for (int i : neg) {
    while (ptr < pos.size() && comp(pos[ptr], i)) ++ptr;
    rp[i] = ptr;
  }
  for (int i = 1; i <= n; ++i)
    if (x[i] && !y[i]) ans += (x[i] < 0 ? pgn : ngp);
  for (int i : pos) ans += i64(lp[i]) * rp[i];
  for (int i : neg) ans += i64(lp[i]) * rp[i];

  std::cout << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}