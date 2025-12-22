#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5;
const i64 INF = 1E18;

int n, q;
i64 ans;
std::string s, t;
std::array<int, N> to, mt;
std::array<i64, N> l, r, p;
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

std::array<i64, N> dis;
std::array<bool, N> vis;
std::priority_queue<std::pair<i64, int>> pq;
std::array<std::vector<std::pair<int, i64>>, N> adj;

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

  for (int i = 0; i < n; ++i) adj[i].clear();
  for (int i = 0; i < n; ++i) {
    if (i) adj[i].emplace_back(i - 1, l[i]);
    if (i + 1 < n) adj[i].emplace_back(i + 1, r[i]);
    adj[i].emplace_back(mt[i], p[i]);
  }

  for (int i = 0; i < q; ++i) {
    for (int i = 0; i < n; ++i) dis[i] = INF, vis[i] = false;
    pq.emplace(dis[qu[i]] = 0, qu[i]);
    for (int u; pq.size();) {
      if (u = pq.top().second, pq.pop(); vis[u]) continue;
      vis[u] = true;
      for (auto [v, w] : adj[u])
        if (dis[v] > dis[u] + w) pq.emplace(-(dis[v] = dis[u] + w), v);
    }
    ans += dis[qv[i]];
  }
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