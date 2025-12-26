#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5;

int n, m;
i64 ans;
std::array<int, N> w;
std::array<std::set<int>, N> adj;

std::vector<int> big;
std::array<int, N> id;
std::vector<int> lp, rp;
std::vector<std::vector<int>> ch;
std::array<std::vector<int>, N> graf;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m, w.fill(1);
  for (int u, v; m; --m) {
    std::cin >> u >> v;
    adj[u].insert(v), adj[v].insert(u);
  }

  std::queue<int> tp;
  for (int i = 1; i <= n; ++i)
    if (adj[i].size() == 1) tp.push(i);
  for (int u, v; tp.size();) {
    if (u = tp.front(), tp.pop(); adj[u].empty()) continue;
    ans += i64(n - w[u]) * w[u];
    adj[v = *begin(adj[u])].erase(u), adj[u].clear(), w[v] += w[u], w[u] = 0;
    if (adj[v].size() == 1) tp.push(v);
  }

  for (int i = 1; i <= n; ++i) {
    if (!w[i]) continue;
    for (int j : adj[i])
      if (w[j]) graf[i].push_back(j);
  }

  for (int i = 1; i <= n; ++i)
    if (graf[i].size() > 2) id[i] = big.size(), big.push_back(i);
  if (big.empty())
    for (int i = 1; i <= n; ++i)
      if (w[i]) big = {i};
  for (int u : big) {
    for (int v : graf[u]) {
      if (graf[v].size() > 2) continue;
      std::vector<int> cur;
      for (int lst = u; graf[v].size() == 2 && v - u;) {
        int nxt = graf[v][graf[v][0] == lst];
        cur.push_back(v), lst = v, v = nxt;
      }
      if (u < v || (u == v && cur[0] <= cur.back()))
        ch.push_back(cur), lp.push_back(u), rp.push_back(v);
    }
  }

  std::vector<std::vector<int>> dis;
  for (int i : big) {
    std::queue<int> bfs;
    dis.push_back(std::vector(n + 1, -1));
    bfs.push(i), dis.back()[i] = 0;
    for (int u; bfs.size();) {
      u = bfs.front(), bfs.pop();
      int d = dis.back()[u] + 1;
      for (int v : graf[u])
        if (dis.back()[v] < 0) dis.back()[v] = d, bfs.push(v);
    }
    for (int j = 1; j <= n; ++j) {
      if (dis.back()[j] < 0) continue;
      if (graf[j].size() == 2 || id[j] > id[i])
        ans += i64(w[i]) * w[j] * dis.back()[j];
    }
  }

  auto eval = [&](int i, int j) {
    int n = ch[i].size(), m = ch[j].size();
    int a = lp[i], b = rp[i], c = lp[j], d = rp[j];
    int ac = dis[id[a]][c], ad = dis[id[a]][d];
    int bc = dis[id[b]][c], bd = dis[id[b]][d];
    std::vector<i64> s0(m + 1), s1(m + 1);
    for (int i = 0; i < m; ++i) {
      s0[i + 1] = s0[i] + w[ch[j][i]];
      s1[i + 1] = s1[i] + i64(w[ch[j][i]]) * i;
    }
    for (int u = 0; u < n; ++u) {
      int gc = std::min(u + 1 + ac, n - u + bc);
      int gd = std::min(u + 1 + ad, n - u + bd);
      int sep = (gd + m - gc + 1) >> 1;
      sep = std::min(std::max(sep, 0), m);
      i64 cur = (gc + 1) * s0[sep] + s1[sep];
      cur += (gd + m) * (s0[m] - s0[sep]) - (s1[m] - s1[sep]);
      ans += w[ch[i][u]] * cur;
    }
  };
  for (int i = 0; i < ch.size(); ++i)
    for (int j = i + 1; j < ch.size(); ++j) eval(i, j);

  for (int i = 0; i < ch.size(); ++i) {
    int n = ch[i].size(), x = n + 1 + dis[id[lp[i]]][rp[i]];
    std::vector<i64> s0(n + 1), s1(n + 1);
    for (int v = 0; v < n; ++v) {
      s0[v + 1] = s0[v] + w[ch[i][v]];
      s1[v + 1] = s1[v] + i64(v) * w[ch[i][v]];
    }
    for (int u = 0; u < n; ++u) {
      int sep = (x >> 1) + u + 1;
      sep = std::min(std::max(sep, u + 1), n);
      i64 cur = s1[sep] - s1[u + 1] - (s0[sep] - s0[u + 1]) * u;
      cur += (x + u) * (s0[n] - s0[sep]) - (s1[n] - s1[sep]);
      ans += i64(cur) * w[ch[i][u]];
    }
  }

  std::cout << ans << '\n';

  return 0;
}