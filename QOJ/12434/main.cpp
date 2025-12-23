#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 3E5 + 5;
const i64 INF = 1E18;

int n, q;
i64 ans;
std::string s, t;
std::array<int, N> to, mt;
std::array<i64, N> l, r, p, qry;
void Parse(int l, int r) {
  if (mt[l] == r) {
    to[l] = s.size(), s += '(';
    if (l + 1 < r) {
      Parse(l + 1, mt[l + 1]);
      if (mt[l + 1] + 1 < r) Parse(mt[l + 1] + 1, r - 1);
    }
    return to[r] = s.size(), void(s += ')');
  }
  s += '(', Parse(l, mt[l]), Parse(mt[l] + 1, r), s += ')';
}

std::array<int, N> qu, qv;
std::array<std::vector<std::pair<int, i64>>, N> adj, rev;

int et;
std::array<std::vector<std::pair<int, int>>, N> spt;
void Spt(int l, int r) {
  for (int i = l + 1; i < r; i = mt[i] + 1)
    spt[l].emplace_back(i, ++et), spt[i].emplace_back(l, et), Spt(i, mt[i]);
}

std::array<bool, N> ban;

std::vector<int> cur, lp, rp, all;
std::array<int, N> siz, fa, tfa, col;
void Get(int u, int fa) {
  siz[u] = 1, cur.push_back(u), all.push_back(u), all.push_back(mt[u]);
  for (auto [v, w] : spt[u])
    if (v - fa && !ban[w]) Get(v, u), siz[u] += siz[v], ::fa[v] = u, tfa[v] = w;
}
void Pull(int u, int fa, std::vector<int> &vc) {
  vc.push_back(u), vc.push_back(mt[u]);
  for (auto [v, w] : spt[u])
    if (v - fa && !ban[w]) Pull(v, u, vc);
}

std::vector<std::tuple<int, int, i64>> ext, qqr;
std::array<std::vector<std::pair<int, int>>, N> hg;

std::array<bool, N> vis;
std::array<i64, N> dis, sid;
std::priority_queue<std::pair<i64, int>> pq;

void Dfs(int u) {
  all.clear(), cur.clear(), Get(u, -1);
  if (siz[u] == 1) return;
  int ku = -1, kv = -1;
  for (int i : cur) {
    if (2 * siz[i] > siz[u]) continue;
    if (ku < 0 || siz[i] > siz[ku]) ku = i, kv = fa[i];
  }
  ban[tfa[ku]] = true;
  lp.clear(), rp.clear(), Pull(ku, -1, lp), Pull(kv, -1, rp);
  for (int i : lp) col[i] = 1;
  for (int i : rp) col[i] = 2;
  ext.clear(), qqr.clear();
  for (int i : all)
    for (auto [v, w] : hg[i])
      if (col[v]) qqr.emplace_back(i, v, w);
  for (int i : lp)
    for (auto [v, w] : adj[i])
      if (col[v] == 2) ext.emplace_back(i, v, w);
  for (int i : rp)
    for (auto [v, w] : adj[i])
      if (col[v] == 1) ext.emplace_back(i, v, w);
  for (auto [u, v, w] : ext) {
    for (int i : all) vis[i] = false, dis[i] = sid[i] = INF;
    pq.emplace(dis[u] = 0, u);
    for (int u; pq.size();) {
      if (u = pq.top().second, pq.pop(); vis[u]) continue;
      vis[u] = true;
      for (auto [v, w] : rev[u])
        if (col[v] && dis[v] > dis[u] + w)
          pq.emplace(-(dis[v] = dis[u] + w), v);
    }
    for (int i : all) vis[i] = false;
    pq.emplace(sid[v] = 0, v);
    for (int u; pq.size();) {
      if (u = pq.top().second, pq.pop(); vis[u]) continue;
      vis[u] = true;
      for (auto [v, w] : adj[u])
        if (col[v] && sid[v] > sid[u] + w)
          pq.emplace(-(sid[v] = sid[u] + w), v);
    }
    for (auto [x, y, z] : qqr) qry[z] = std::min(qry[z], dis[x] + sid[y] + w);
  }
  for (int i : all) col[i] = 0;
  Dfs(ku), Dfs(kv);
}

void Proc(int ks) {
  std::cin >> n >> q >> t, ans = 0;
  std::vector<int> brc;
  for (int i = 0; i < n; ++i)
    if (t[i] == '(')
      brc.push_back(i);
    else
      mt[mt[brc.back()] = i] = brc.back(), brc.pop_back();
  s.clear(), Parse(0, n - 1), brc.clear();
  for (int i = 0; i < s.size(); ++i)
    if (s[i] == '(')
      brc.push_back(i);
    else
      mt[mt[brc.back()] = i] = brc.back(), brc.pop_back();
  for (int i = 0; i < s.size(); ++i) l[i] = r[i] = 0, p[i] = INF;
  for (int i = 0; i < n; ++i) std::cin >> l[to[i]];
  for (int i = 0; i < n; ++i) std::cin >> r[to[i]];
  for (int i = 0; i < n; ++i) std::cin >> p[to[i]];
  n = s.size();
  for (int i = 0; i < q; ++i) std::cin >> qu[i], qu[i] = to[--qu[i]];
  for (int i = 0; i < q; ++i) std::cin >> qv[i], qv[i] = to[--qv[i]];

  for (int i = 0; i < n; ++i) adj[i].clear(), rev[i].clear(), spt[i].clear();
  for (int i = 0; i < n; ++i) {
    if (i) adj[i].emplace_back(i - 1, l[i]);
    if (i + 1 < n) adj[i].emplace_back(i + 1, r[i]);
    adj[i].emplace_back(mt[i], p[i]);
  }
  for (int u = 0; u < n; ++u)
    for (auto [v, w] : adj[u]) rev[v].emplace_back(u, w);

  et = 0, Spt(0, n - 1);
  for (int i = 1; i <= et; ++i) ban[i] = false;
  for (int i = 0; i < n; ++i) hg[i].clear();
  for (int i = 0; i < q; ++i) {
    qry[i] = INF, hg[qu[i]].emplace_back(qv[i], i);
    if (qu[i] == qv[i]) qry[i] = 0;
    for (auto [v, w] : adj[qu[i]])
      if (v == qv[i]) qry[i] = std::min(qry[i], w);
  }
  Dfs(0);
  for (int i = 0; i < q; ++i) ans += qry[i];
  std::cout << "Case #" << ks << ": " << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  for (int ks = 1; ks <= t; ++ks) Proc(ks);

  return 0;
}