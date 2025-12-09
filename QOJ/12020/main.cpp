#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E6 + 5;
std::mt19937_64 rng(20090627);

int n, q;
std::array<int, N> a;
std::array<std::array<int, N>, 20> mx;
int Max(int l, int r) {
  int k = std::__lg(r - l + 1);
  return std::max(mx[k][l], mx[k][r - (1 << k) + 1]);
}

int d;
std::array<int, N> dfn, low;
std::array<std::vector<int>, N> adj;
std::array<std::array<int, 20>, N> jp;
void Dfs(int u, int fa) {
  dfn[u] = ++d, jp[u][0] = fa;
  for (int i = 1; i < 20; ++i) jp[u][i] = jp[jp[u][i - 1]][i - 1];
  for (int v : adj[u]) Dfs(v, u);
  low[u] = d;
}
std::array<i64, N> bit;
void Add(int x, int v) {
  for (; x <= d; x += x & -x) bit[x] += v;
}
void Add(int l, int r, int v) { Add(l, v), Add(r + 1, -v); }
i64 Query(int x) {
  i64 v = 0;
  for (; x; x &= x - 1) v += bit[x];
  return v;
}

struct I {
  int lc, rc, sz, bp, rp;
  u64 pro;
};
int rt, id;
std::array<I, N * 2> t;
void Up(int o) { t[o].sz = t[t[o].lc].sz + t[t[o].rc].sz + 1; }
std::pair<int, int> Split(int o, int s) {
  if (!s) return {0, o};
  if (s == t[o].sz) return {o, 0};
  if (s > t[t[o].lc].sz) {
    auto [l, r] = Split(t[o].rc, s - t[t[o].lc].sz - 1);
    return t[o].rc = l, Up(o), std::pair{o, r};
  }
  auto [l, r] = Split(t[o].lc, s);
  return t[o].lc = r, Up(o), std::pair{l, o};
}
int Merge(int u, int v) {
  if (!u || !v) return u ^ v;
  if (t[u].pro > t[v].pro) return t[u].rc = Merge(t[u].rc, v), Up(u), u;
  return t[v].lc = Merge(u, t[v].lc), Up(v), v;
}
i64 Ans(int l, int r) {
  auto rp = [&](int x) {
    for (int o = rt;;) {
      if (x == t[t[o].lc].sz + 1) return t[o].rp;
      if (x > t[t[o].lc].sz)
        x -= t[t[o].lc].sz + 1, o = t[o].rc;
      else
        o = t[o].lc;
    }
  };
  int rpl = rp(l), rpr = l == r ? rpl : rp(r);
  int max = Max(l, rpl);
  if (l == r) return max;
  int u = l, v;
  for (int i = 19; ~i; --i)
    if (jp[u][i] <= rpr && ::a[jp[u][i]] <= max) u = jp[u][i];
  v = u = jp[u][0];
  for (int i = 19; ~i; --i)
    if (jp[v][i] <= rpr) v = jp[v][i];
  if (v <= rpr) v = jp[v][0];
  return Query(dfn[u]) - Query(dfn[v]) + max;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> q;
  for (int i = 1; i <= n; ++i) std::cin >> a[i];

  mx[0] = a;
  for (int i = 1; i < 20; ++i)
    for (int j = 1; j + (1 << i) - 1 <= n; ++j)
      mx[i][j] = std::max(mx[i - 1][j], mx[i - 1][j + (1 << (i - 1))]);

  std::vector stk{n + 1};
  for (int i = n; i; --i) {
    while (stk.size() > 1 && a[stk.back()] < a[i]) stk.pop_back();
    adj[stk.back()].emplace_back(i), stk.emplace_back(i);
  }
  Dfs(n + 1, n + 1);
  for (int i = 1; i <= n; ++i) Add(dfn[i], low[i], a[i]);

  for (int i = 1; i <= (id = n); ++i)
    t[i].rp = t[i].bp = i, t[i].pro = rng(), t[i].sz = 1, rt = Merge(rt, i);

  for (int o, l, r; q; --q) {
    std::cin >> o >> l >> r;
    if (o == 1) {
      auto [a, b] = Split(rt, r);
      auto [c, d] = Split(a, l - 1);
      auto [e, f] = Split(d, 1);
      int o = f;
      while (t[o].lc) o = t[o].lc;
      if (::a[t[o].bp] > ::a[t[e].bp]) {
        if (t[e].bp) Add(dfn[t[e].bp], low[t[e].bp], -::a[t[e].bp]);
      } else
        t[o].bp = t[e].bp;
      for (o = f; t[o].rc; o = t[o].rc);
      t[++id].sz = 1, t[id].pro = rng(), t[id].rp = t[o].rp;
      rt = Merge(Merge(c, f), Merge(id, b));
    } else
      std::cout << Ans(l, r) << '\n';
  }

  return 0;
}