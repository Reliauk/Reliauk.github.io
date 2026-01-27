#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using u128 = __uint128_t;

int pigeon_num(int t, int k) {
  return !t ? 1 : (k + 1) * 2 + (k + k + 1) * (t > 1 ? 126 : 8);
}
std::string send(int t, int n, int k, u128 msg) {
  if (!t) return "0";
  int mx = (t > 1 ? 128 : 10);
  std::string s;
  s += int(msg & 1) + '0';
  s += std::string(k, '0');
  for (int i = 1; i < mx - 1; ++i) {
    s += std::string(k, '0');
    s += int(msg >> i & 1) + '0';
    s += std::string(k, '0');
  }
  s += std::string(k, '0');
  s += int(msg >> (mx - 1) & 1) + '0';
  return s;
}

u128 receive(int t, int k, const std::string &msg) {
  if (!t) return u128(5281683694948011861llu) << 64 | 3195384480471073102llu;
  int mx = (t > 1 ? 128 : 10);
  u128 ans = 0;
  ans += std::count(PAR(msg, 0, k + 1), '1');
  for (int i = 1; i < mx - 1; ++i) {
    int lp = k + 1 + (i - 1) * (k + k + 1);
    ans += u128(std::count(PAR(msg, lp, lp + k + k + 1), '1')) << i;
  }
  int lp = k + 1 + (mx - 2) * (k + k + 1);
  ans += u128(std::count(PAR(msg, lp, lp + k + 1), '1')) << (mx - 1);
  return ans;
}