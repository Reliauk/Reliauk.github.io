#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r
#include "cheat.h"

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using D = std::pair<int, int>;
using S = std::pair<D, D>;
using P = std::vector<S>;

const P ord{
    {{0, 0}, {0, 1}}, {{0, 2}, {1, 2}}, {{2, 1}, {2, 2}}, {{1, 0}, {2, 0}},
    {{0, 0}, {1, 0}}, {{0, 1}, {0, 2}}, {{1, 2}, {2, 2}}, {{2, 0}, {2, 1}},
    {{0, 0}, {1, 1}}, {{0, 2}, {1, 1}}, {{1, 1}, {2, 2}}, {{1, 1}, {2, 0}},
    {{0, 0}, {1, 2}}, {{0, 2}, {2, 1}}, {{1, 0}, {2, 2}}, {{0, 1}, {2, 0}},
    {{0, 0}, {2, 1}}, {{0, 2}, {1, 0}}, {{0, 1}, {2, 2}}, {{1, 2}, {2, 0}},
    {{0, 1}, {1, 0}}, {{0, 1}, {1, 2}}, {{1, 2}, {2, 1}}, {{1, 0}, {2, 1}},
    {{1, 0}, {1, 1}}, {{0, 1}, {1, 1}}, {{1, 1}, {1, 2}}, {{1, 1}, {2, 1}}};
P Encode(int s) {
  P p;
  for (; s; s &= s - 1) p.push_back(ord[__builtin_ctz(s)]);
  return p;
}
int Decode(P p) {
  int s = 0;
  for (auto [x, y] : p) {
    if (x > y) std::swap(x, y);
    s ^= 1 << (std::find(ALL(ord), std::pair{x, y}) - begin(ord));
  }
  return s;
}

P BuildPattern(int n) {
  for (int i = 0; i < 7; ++i) {
    int cur = 3 << (24 - i - i);
    if (n >= cur) {
      n -= cur;
      continue;
    }
    int ans = 0;
    for (int j = 0; j <= n >> (24 - i - i); ++j) ans ^= 1 << (i * 4 + j);
    n &= (1 << (24 - i - i)) - 1;
    for (int j = 0; j < i; ++j, n >>= 2) {
      if (n & 1) ans ^= 5 << (j * 4);
      if (n & 2) ans ^= 10 << (j * 4);
    }
    ans ^= n << ((i + 1) * 4);
    return Encode(ans);
  }
  __builtin_unreachable();
}
int GetCardNumber(P p) {
  int s = Decode(p), sum = 0;
  for (int i = 0; i < 7; ++i) {
    int cur = s >> (i * 4) & 15;
    if (!cur || cur == 5 || cur == 10 || cur == 15) {
      sum += 3 << (24 - i - i);
      continue;
    }
    auto shift = [&](int x, int s) { return x <<= s, (x & 15) ^ (x >> 4); };
    int c = __builtin_popcount(cur), sh = 0;
    sum += (c - 1) << (24 - i - i);
    while (shift((1 << c) - 1, sh) - cur) ++sh;
    for (int i = 0; i < 7; ++i) {
      int x = s >> (i * 4) & 15;
      s ^= (x ^ shift(x, 4 - sh)) << (i * 4);
    }
    int n = s >> ((i + 1) * 4);
    for (int j = i - 1; ~j; --j) {
      int x = s >> (j * 4) & 15;
      n <<= 2;
      if (x & 5) n ^= 1;
      if (x & 10) n ^= 2;
    }
    return sum + n;
  }
  __builtin_unreachable();
}