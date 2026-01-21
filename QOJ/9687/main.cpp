#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r
#define GET(t, i) std::get<i>(t)

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 4E5 + 5;
const i64 INF = 4E18;

int n, m, p;
std::array<std::vector<int>, N> adj;
std::array<std::unordered_map<int, int>, N> wt;

int d, k;
std::vector<int> anc;
std::array<int, N> dfn, low;
std::array<std::vector<int>, N> spt;
void Dfs(int u) {
  dfn[u] = low[u] = ++d, anc.push_back(u);
  for (int v : adj[u])
    if (!dfn[v]) {
      Dfs(v), low[u] = std::min(low[u], low[v]);
      if (low[v] < dfn[u]) continue;
      spt[u].push_back(++k);
      for (int w = -1; w - v; anc.pop_back()) spt[k].push_back(w = anc.back());
    } else
      low[u] = std::min(low[u], dfn[v]);
}

std::array<std::array<i64, 3>, N> f;
i64 S(int n) { return i64(n - 1) * n >> 1; }
i64 W(int u, int b) {
  int n = adj[u].size();
  return p * ((n - b) * S(n) - n * S(n - b));
}
void Dp(int u, int fa) {
  for (int v : spt[u]) Dp(v, u);
  f[u].fill(-INF);
  if (u > n) {
    if (spt[u].size() == 1) {
      f[u][0] = f[spt[u][0]][0], f[u][1] = f[spt[u][0]][1] - wt[spt[u][0]][fa];
      return;
    }
    for (int lp : {0, 1}) {
      std::array<i64, 2> dp{-INF, -INF};
      dp[lp] = -lp * wt[spt[u][0]][fa];
      for (int i = 0; i < spt[u].size(); ++i) {
        std::array<i64, 2> nxt{-INF, -INF};
        int e = wt[spt[u][i]][i + 1 < spt[u].size() ? spt[u][i + 1] : fa];
        for (int l : {0, 1})
          for (int r : {0, 1})
            nxt[r] = std::max(nxt[r], dp[l] + f[spt[u][i]][l + r] - r * e);
        dp.swap(nxt);
      }
      for (int i : {0, 1}) f[u][lp + i] = std::max(f[u][lp + i], dp[i]);
    }
    return;
  }
  i64 base = 0;
  std::vector<std::tuple<int, i64, i64>> it;
  for (int v : spt[u]) {
    base += f[v][0];
    i64 a = f[v][1] - f[v][0];
    i64 b = f[v][2] - f[v][1];
    if (a >= b)
      it.emplace_back(1, a, 0), it.emplace_back(1, b, 0);
    else
      it.emplace_back(2, a + b, b);
  }
  sort(ALL(it), [&](auto u, auto v) {
    return GET(u, 1) * GET(v, 0) > GET(v, 1) * GET(u, 0);
  });
  int cnt = 0, ptr = 0;
  i64 sum = 0, mo = INF, mt = INF, mtt = INF;
  std::vector<i64> nxt(it.size() + 1);
  nxt.back() = -INF;
  for (int i = it.size() - 1; ~i; --i)
    nxt[i] = GET(it[i], 0) > 1 ? nxt[i + 1] : GET(it[i], 1);
  for (int k = 0; k <= 2 * spt[u].size(); ++k) {
    while (cnt < k) {
      auto [c, s, e] = it[ptr++];
      if (c > 1)
        mt = s, mtt = std::min(mtt, e);
      else
        mo = s;
      cnt += c, sum += s;
    }
    i64 dp = sum + base;
    if (cnt > k) dp -= std::min({mo, mtt, mt - nxt[ptr]});
    for (int i : {0, 1, 2})
      if (k + i <= adj[u].size()) f[u][i] = std::max(f[u][i], dp + W(u, k + i));
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m >> p;
  for (int u, v, w; m; --m) {
    std::cin >> u >> v >> w;
    adj[u].push_back(v), adj[v].push_back(u);
    wt[u][v] = wt[v][u] = w;
  }

  k = n, Dfs(1), Dp(1, 0);
  std::cout << f[1][0] << '\n';

  return 0;
}