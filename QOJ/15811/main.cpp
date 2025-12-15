#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5;

int n, m, k = 30;
std::array<std::vector<int>, N> adj;

std::vector<int> v;
std::array<bool, N> vis, us;
void Dfs(int u) {
  v.push_back(u), vis[u] = true;
  for (int v : adj[u])
    if (!vis[v]) Dfs(v);
}

int sum;
std::array<int, N> id, id2, pu, pt;
std::array<std::vector<int>, N> sub;
int Proc() {
  if (v.size() & 1) {
    for (int i = 0; i < v.size(); ++i) id2[v[i]] = i + 1;
    return 0;
  }
  auto tmp = v;
  for (int i : v) us[i] = true, sub[i].clear();
  int min = -1;
  for (int i : v) {
    for (int j : adj[i])
      if (us[j]) sub[i].push_back(j);
    if (min < 0 || sub[i].size() < sub[min].size()) min = i;
  }
  auto run = [&](int to) {
    std::queue<int> bfs;
    for (int i : v) pu[i] = 0, pt[i] = 0;
    bfs.push(min), pu[min] = 2, pu[pt[min] = to] = 2;
    for (int u; bfs.size();) {
      u = bfs.front(), bfs.pop();
      for (int v : sub[pt[u]])
        if (!pu[v]) pu[v] = 1;
      for (int v : sub[u]) {
        if (pu[v]) continue;
        pu[v] = 2, bfs.push(v);
        std::vector<int> nxt;
        for (int w : sub[v])
          if (pu[w] == 1) nxt.push_back(w);
        if (nxt.size() != 1) return 0;
        pu[pt[v] = nxt[0]] = 2;
      }
    }
    std::vector<int> lp, rp;
    for (int i : v) (pt[i] ? lp : rp).push_back(i);
    if (lp.size() != v.size() / 2 || rp.size() != v.size() / 2) return 0;
    std::set<std::pair<int, int>> sl, sr;
    for (int i : v)
      for (int j : sub[i]) {
        if (!pt[i] && !pt[j]) sl.emplace(i, j);
        if (pt[i] && pt[j]) sr.emplace(pt[i], pt[j]);
      }
    if (sl != sr) return 0;
    for (int i : tmp) us[i] = false;
    v = rp;
    int ret = Proc() + 1;
    for (int i : lp) id[i] = id[pt[i]] * 2, id2[i] = id2[pt[i]];
    for (int i : rp) id[i] += id[i] + 1;
    return ret;
  };
  for (int to : sub[min])
    if (int ans = run(to); ans) {
      for (int i : tmp) us[i] = false;
      return ans;
    }
  for (int i = 0; i < v.size(); ++i) id2[v[i]] = i + 1;
  for (int i : tmp) us[i] = false;
  return 0;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int u, v; m; --m) {
    std::cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  std::vector<int> dep;
  std::vector<std::vector<int>> tmp;
  for (int i = 1, cur; i <= n; ++i) {
    if (vis[i]) continue;
    v.clear(), Dfs(i), tmp.push_back(v);
    k = std::min(k, cur = Proc()), dep.push_back(cur);
  }
  std::cout << k << '\n';
  if (!k) exit(0);

  int sum = 0;
  for (int i = 0; i < tmp.size(); ++i) {
    for (int j : tmp[i])
      id2[j] = ((id2[j] - 1) << (dep[i] - k)) + (id[j] >> k) + sum + 1;
    sum += tmp[i].size() >> k;
  }
  for (int i = 1; i <= n; ++i) {
    std::cout << id2[i] << ' ';
    for (int j = 0; j < k; ++j) std::cout << (id[i] >> j & 1);
    std::cout << '\n';
  }

  return 0;
}