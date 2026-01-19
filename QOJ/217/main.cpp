#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int S = 4E5 + 5;

int d = 1, pf = 1;
std::array<int, S> fa, len;
std::array<std::array<int, 26>, S> sam;
void Extend(int c) {
  int cur = pf;
  len[pf = ++d] = len[cur] + 1;
  while (cur && !sam[cur][c]) sam[cur][c] = pf, cur = fa[cur];
  if (!cur) return void(fa[pf] = 1);
  int nxt = sam[cur][c];
  if (len[cur] + 1 == len[nxt]) return void(fa[pf] = nxt);
  int up = ++d;
  sam[up] = sam[nxt], len[up] = len[cur] + 1, fa[up] = fa[nxt];
  fa[nxt] = fa[pf] = up;
  while (cur && sam[cur][c] == nxt) sam[cur][c] = up, cur = fa[cur];
}

std::array<int, S> edp;
std::array<std::vector<int>, S> adj;
void Dfs(int u) {
  for (int v : adj[u]) Dfs(v), edp[u] += edp[v];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string s;
  std::cin >> s;
  for (char c : s) Extend(c - 'a');
  for (int i = 0, u = 1; i < s.size(); ++i) ++edp[u = sam[u][s[i] - 'a']];
  for (int i = 2; i <= d; ++i) adj[fa[i]].push_back(i);
  Dfs(1);
  int ans = 0;
  for (int i = 1; i <= d; ++i)
    if (edp[i] > 1) ans = std::max(ans, edp[i] * len[i]);
  std::cout << ans << '\n';

  return 0;
}