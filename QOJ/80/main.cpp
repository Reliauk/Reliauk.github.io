#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5, MOD = 1E9 + 7;
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}
std::array<int, N> pw;
void Math() {
  for (int i = pw[0] = 1; i < N; ++i) Add(pw[i] = pw[i - 1], pw[i - 1]);
}

int h, w, n, ans;
std::array<int, N> row, col;
std::array<std::array<int, 2>, N> sr, sc;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  Math();

  std::cin >> h >> w >> n, row.fill(-1), col.fill(-1);
  for (int x, y, d; n; --n) {
    std::cin >> x >> y >> d;
    int rv = (d == 2 || d == 3) ^ (y - 1) & 1;
    int cv = (d > 2) ^ (x - 1) & 1;
    if (~row[x] && row[x] - rv || ~col[y] && col[y] - cv)
      std::cout << "0\n", exit(0);
    row[x] = rv, col[y] = cv;
  }

  for (int o : {0, 1}) {
    for (int i = 1; i <= h; ++i)
      sr[i][o] = sr[i - 1][o] + (row[i] < 0 || row[i] == (o ^ i & 1));
    for (int i = 1; i <= w; ++i)
      sc[i][o] = sc[i - 1][o] + (col[i] < 0 || col[i] == (o ^ i & 1));
  }

  bool rc = sr[h][0] == h || sr[h][1] == h;
  bool cc = sc[w][0] == w || sc[w][1] == w;
  int nr = std::count(PAR(row, 1, h + 1), -1);
  int nc = std::count(PAR(col, 1, w + 1), -1);
  if (rc && cc) ans = MOD - ((nr == h) + 1) * ((nc == w) + 1);
  if (rc) Add(ans, pw[nc + (nr == h)]);
  if (cc) Add(ans, pw[nr + (nc == w)]);

  std::array<int, 2> val{0, 0};
  for (int o : {0, 1})
    for (int j = 1; j < h; ++j)
      Add(val[o], sr[j][o] == j && sr[h][!o] - sr[j][!o] == h - j);
  for (int i = 1; i < w; ++i)
    for (int o : {0, 1})
      if (sc[i][o ^ i & 1] == i &&
          sc[w][!o ^ i & 1] - sc[i][!o ^ i & 1] == w - i)
        Add(ans, val[!o ^ (i & 1)]);

  std::cout << ans << '\n';

  return 0;
}