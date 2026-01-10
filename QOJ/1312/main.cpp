#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E6 + 5;

int n, m;
std::array<int, N> a, b;
std::array<std::vector<std::pair<int, int>>, N> adj;

int sa, sb;
std::vector<int> cur;
std::array<bool, N> vis;
std::array<int, N> da, db;
std::tuple<int, int, int> ext;
std::vector<std::pair<int, int>> ch;
void Dfs(int u, int e) {
  vis[u] = true, cur.push_back(u);
  for (auto [v, w] : adj[u])
    if (!vis[v]) {
      da[v] = da[u], db[v] = db[u];
      if (u < v)
        sb += b[w], da[v] += a[w], db[v] -= b[w];
      else
        sa += a[w], da[v] -= a[w], db[v] += b[w];
      Dfs(v, w);
    } else if (e - w)
      ext = {v, u, w};
}

int bx, by;
std::vector<std::pair<int, int>> ans;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int i = 1, x, y; i <= m; ++i) {
    std::cin >> x >> y >> a[i] >> b[i];
    adj[x].emplace_back(y + n, i);
    adj[y + n].emplace_back(x, i);
  }

  for (int i = 1; i <= n + n; ++i) {
    if (vis[i]) continue;
    ch.clear(), cur.clear(), ext = {0, 0, 0}, sa = sb = 0, Dfs(i, 0);
    if (std::get<0>(ext)) {
      auto [u, v, w] = ext;
      if (u > v) std::swap(u, v);
      for (int i : cur) vis[i] = false;
      cur.clear(), ext = {0, 0, 0}, sa = sb = 0, da[u] = db[u] = 0, Dfs(u, 0);
      if (std::get<0>(ext) - u) std::swap(std::get<0>(ext), std::get<1>(ext));
      v = std::get<1>(ext), w = std::get<2>(ext);
      ch.emplace_back(sa + a[w], sb);
      ch.emplace_back(sa + da[v], sb + b[w] + db[v]);
    } else {
      for (int i : cur) ch.emplace_back(da[i] + sa, db[i] + sb);
    }
    sort(ALL(ch));
    std::vector<std::pair<int, int>> nxt;
    for (auto [x, y] : ch) {
      while (nxt.size() >= 2) {
        auto [a, b] = nxt.back();
        auto [c, d] = end(nxt)[-2];
        if (i64(c - a) * (y - b) >= i64(x - a) * (d - b))
          nxt.pop_back();
        else
          break;
      }
      nxt.emplace_back(x, y);
    }
    bx += nxt[0].first, by += nxt[0].second;
    for (int i = 1; i < nxt.size(); ++i)
      ans.emplace_back(nxt[i].first - nxt[i - 1].first,
                       nxt[i].second - nxt[i - 1].second);
  }
  sort(ALL(ans), [&](auto p, auto q) {
    return i64(p.second) * q.first < i64(q.second) * p.first;
  });
  i64 out = i64(bx) * by;
  for (auto [x, y] : ans) out = std::min(out, i64(bx += x) * (by += y));
  std::cout << out << '\n';

  return 0;
}