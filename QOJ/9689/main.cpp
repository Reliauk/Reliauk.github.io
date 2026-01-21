#include <bits/stdc++.h>

#include "prg.h"
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

int solve(int n, int m, int k, std::vector<int> op,
          std::array<std::vector<int>, 3000> x, int c,
          std::array<std::array<int, 3000>, 25> s) {
  double sum = 0;
  for (int i = 0; i < c; ++i)
    for (int j = 0; j < m; ++j) sum += s[i][j];
  sum /= m * c;

  if (sum < 0.48 || sum > 0.52) return 2;
  if (op.back() != 2) return 1;

  std::array<std::vector<int>, 1000> qry;
  for (int i = 0; i < m; ++i) qry[x[i].back()].push_back(i);
  sum = 0;
  double all = 0;
  for (int i = 0; i < n; ++i) {
    if (qry[i].size() < 2) continue;
    for (int j = 0; j < c; ++j)
      for (int k = 1; k < qry[i].size(); ++k)
        sum += s[j][qry[i][k]] != s[j][qry[i][0]], ++all;
  }
  sum /= all;
  if (std::abs(sum - 0.5) > 0.02) return 2;
  if (k == 3) return 1;

  std::map<std::pair<int, int>, std::vector<int>> qry2;
  for (int i = 0; i < m; ++i) qry2[std::minmax(x[i][2], x[i][3])].push_back(i);
  sum = all = 0;
  for (auto [_, v] : qry2) {
    if (v.size() < 2) continue;
    for (int j = 0; j < c; ++j)
      for (int k = 1; k < v.size(); ++k) sum += s[j][v[k]] != s[j][v[0]], ++all;
  }
  sum /= all;
  if (std::abs(sum - 0.5) > 0.09) return 2;

  return 1;
}