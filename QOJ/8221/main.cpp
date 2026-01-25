#include <bits/stdc++.h>

#include "mpc.h"

#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

int n, m;
int precalc(int n, int m) { return ::n = n, ::m = m, n + m + 3; }
bool transmit(player &p, int r, int i) {
  int pos = r - i - 1;
  if (pos < -8)
    pos += m + 19;
  else if (pos < 0)
    pos += m + 4;
  if (pos < 0) return false;
  p.memory[pos] += p.last_message;
  for (int q = pos; p.memory[q] > 1; ++q) ++p.memory[q + 1], p.memory[q] = 0;
  int ans = p.memory[pos];
  if (i < n) p.memory[pos] = 0;
  return ans;
}