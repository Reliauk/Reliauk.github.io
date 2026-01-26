#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

int r, c, x;
i64 ans;
std::vector<std::tuple<int, int, char>> val;

void Run(int lp, int rp) {
  std::array<std::vector<int>, 2> pre;
  for (int o : {0, 1}) pre[o].resize(rp - lp + 1);
  for (auto [x, y, v] : val) ++pre[(v == '-') ^ ((x + y) & 1)][x + y - lp];
  for (int o : {0, 1}) std::partial_sum(ALL(pre[o]), begin(pre[o]));
  for (int i = lp; i < rp; ++i)
    ans += !pre[~i & 1][i - lp] && pre[i & 1].back() == pre[i & 1][i - lp];
  std::array<int, 2> stt{0, 0};
  for (int i : {0, 1}) {
    stt[i] = lp + ((lp ^ i) & 1);
    while (stt[i] <= rp && pre[i].back() - pre[i][stt[i] - lp]) stt[i] += 2;
  }
  std::vector<int> rpp(rp - lp + 1);
  for (int p = lp; p < rp; ++p) {
    if (p > lp + 1)
      rpp[p - lp] = std::max(p, rpp[p - lp - 2]);
    else
      rpp[p - lp] = p;
    if (pre[~p & 1][p - lp]) continue;
    int st = p + std::max({r, c, 3}) - 1;
    if ((st & 1) == (p & 1)) ++st;
    st = std::max(st, stt[~p & 1]);
    while (rpp[p - lp] < rp &&
           pre[p & 1][rpp[p - lp] - lp] <= pre[p & 1][p - lp])
      ++rpp[p - lp];
    if (st < rpp[p - lp]) ans += (rpp[p - lp] - st - 1) / 2 + 1;
  }
}
void Proc() {
  std::cin >> r >> c >> x, val.resize(x), ans = 0;
  for (auto &[x, y, v] : val) std::cin >> v >> x >> y;

  std::set<int> s;
  for (auto [x, y, v] : val) s.insert((x & 1) ^ (y & 1) ^ (v == '+'));
  if (s.size() < 2) ans += 1 + s.empty();

  Run(2, r + c);
  for (auto &[x, y, v] : val) y = -y;
  if (r > 1 && c > 1) Run(1 - c, r - 1);

  if (r == 2 && c == 2) {
    bool suc = true;
    for (auto [_, __, v] : val) suc &= v == '+';
    ans += suc;
  }
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