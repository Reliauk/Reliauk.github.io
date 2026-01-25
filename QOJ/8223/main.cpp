#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E6 + 5, L = 21;

int d = 1, pf = 1;
std::array<int, N * 2> fa, len;
std::array<std::array<int, 26>, N * 2> sam;
void Append(int c) {
  int cur = pf;
  len[++d] = len[pf] + 1, pf = d;
  while (cur && !sam[cur][c]) sam[cur][c] = pf, cur = fa[cur];
  if (!cur) return void(fa[pf] = 1);
  int nxt = sam[cur][c];
  if (len[cur] + 1 == len[nxt]) return void(fa[pf] = nxt);
  sam[++d] = sam[nxt], len[d] = len[cur] + 1, fa[d] = fa[nxt];
  fa[nxt] = fa[pf] = d;
  while (cur && sam[cur][c] == nxt) sam[cur][c] = d, cur = fa[cur];
}
std::array<int, N * 2> suf, dfn, low;
std::array<std::array<int, L>, N * 2> anc;
std::array<std::vector<int>, N * 2> adj;
void Dfs(int u) {
  dfn[u] = ++d, anc[u][0] = fa[u];
  for (int i = 1; i < L; ++i) anc[u][i] = anc[anc[u][i - 1]][i - 1];
  for (int v : adj[u]) Dfs(v);
  low[u] = d;
}

int n;
std::string s;
std::array<int, N> dp;

std::array<int, N * 2> bit;
void Add(int x, int v) {
  for (; x <= d; x += x & -x) bit[x] += v;
}
int Query(int x) {
  int v = 0;
  for (; x; x &= x - 1) v += bit[x];
  return v;
}
int Occ(int l, int r) {
  int ln = r - l + 1, u = suf[l];
  for (int d = L - 1; ~d; --d)
    if (len[anc[u][d]] >= ln) u = anc[u][d];
  return Query(low[u]) - Query(dfn[u] - 1);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> s, n = s.size();
  for (int i = n - 1; ~i; --i) Append(s[i] - 'a'), suf[i] = pf;
  for (int i = 2; i <= d; ++i) adj[fa[i]].push_back(i);
  d = 0, Dfs(1);

  std::vector<int> stk{n};
  for (int i = n - 1; ~i; --i) {
    dp[i] = 1, Add(dfn[suf[i]], 1);
    while (stk.size()) {
      dp[i] = std::max(dp[i], Occ(i, stk.back() - 1) - dp[stk.back()]);
      if (dp[i] < dp[stk.back()])
        stk.pop_back();
      else
        break;
    }
    stk.push_back(i);
  }

  std::cout << dp[0] << '\n';

  return 0;
}