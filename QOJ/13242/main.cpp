#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using i128 = __int128;

const int N = 1E5 + 5;

int n, m;
std::string str;
std::array<i64, N> hash, ans;
void CountM() {
  const i64 MOD = 989878776171074651, BASE = 20090627;
  for (int i = 0; i < m; ++i) hash[0] = (i128(hash[0]) * BASE + str[i]) % MOD;
  i64 pw = 1;
  for (int i = 1; i < m; ++i) pw = i128(pw) * BASE % MOD;
  for (int i = m; i < n; ++i) {
    hash[i - m + 1] = (hash[i - m] + i128(MOD - pw) * str[i - m]) % MOD;
    hash[i - m + 1] = (i128(hash[i - m + 1]) * BASE + str[i]) % MOD;
  }
  std::map<i64, int> cnt;
  for (int i = 0; i <= n - m; ++i) ++cnt[hash[i]];
  for (int i = 0; i <= n - m; ++i) ans[i] = cnt[hash[i]] - 1;
}

struct A {
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
  std::array<int, N * 2> dfn, low, seq;
  std::array<std::vector<int>, N * 2> adj;
  void Dfs(int u) {
    seq[dfn[u] = ++d] = u;
    for (int v : adj[u]) Dfs(v);
    low[u] = d;
  }
  void Make() {
    for (int i = 2; i <= d; ++i) adj[fa[i]].push_back(i);
    d = 0, Dfs(1);
  }
} s, t;
std::array<int, N * 2> ns, nt, is, it;

std::array<int, N * 2> siz, hs;
void Dfs(int u) {
  siz[u] = 1;
  for (int v : s.adj[u]) {
    Dfs(v), siz[u] += siz[v];
    if (siz[v] > siz[hs[u]]) hs[u] = v;
  }
}

namespace segt {
struct I {
  int l, r, s, t;
};
std::array<I, N * 8> t;
#define LC (o + o)
#define RC (o + o + 1)
void Make(int o, int l, int r) {
  t[o].l = l, t[o].r = r;
  if (l == r) return;
  int m = (l + r) >> 1;
  Make(LC, l, m), Make(RC, m + 1, r);
}
void Upd(int o, int v) { t[o].t += (t[o].s ? v : 0); }
void Push(int o) {
  if (t[o].t) Upd(LC, t[o].t), Upd(RC, t[o].t), t[o].t = 0;
}
void Add(int x, int v) {
  int o = 1;
  while (t[o].l - t[o].r) {
    int m = (t[o].l + t[o].r) >> 1;
    Push(o), t[o].s += v, o = LC + (x > m);
  }
  t[o].s += v;
}
void Add(int o, int x, int y, int v) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return Upd(o, v);
  if (x > r || l > y || x > y) return;
  return Push(o), Add(LC, x, y, v), Add(RC, x, y, v);
}
int Query(int o, int x, int y) {
  int l = t[o].l, r = t[o].r;
  if (x <= l && r <= y) return t[o].s;
  if (x > r || l > y || x > y) return 0;
  return Push(o), Query(LC, x, y) + Query(RC, x, y);
}
#undef LC
#undef RC
}  // namespace segt

std::array<std::array<int, 20>, N * 2> anc;
void Upd(int x, int v) {
  if (is[x] >= 0) segt::Add(t.dfn[nt[is[x]]], v);
}
void Query(int x, int w) {
  if (is[x] < 0) return;
  int tar = m - 1 - w, y = nt[is[x]];
  if (tar >= t.len[y]) return;
  for (int d = 19; ~d; --d)
    if (t.len[anc[y][d]] > tar) y = anc[y][d];
  if (t.len[t.fa[y]] == tar) {
    ans[is[x]] += segt::Query(1, t.dfn[t.fa[y]], t.low[t.fa[y]]);
    ans[is[x]] -= segt::Query(1, t.dfn[y], t.low[y]);
    segt::Add(1, t.dfn[t.fa[y]], t.low[t.fa[y]], 1);
    segt::Add(1, t.dfn[y], t.low[y], -1);
  }
}
void Proc(int u, bool c) {
  for (int v : s.adj[u])
    if (v - hs[u]) Proc(v, true);
  if (hs[u]) Proc(hs[u], false);
  for (int v : s.adj[u]) {
    if (v == hs[u]) continue;
    for (int i = s.dfn[v]; i <= s.low[v]; ++i) Query(s.seq[i], s.len[u]);
    for (int i = s.dfn[v]; i <= s.low[v]; ++i) Upd(s.seq[i], 1);
  }
  Upd(u, 1);
  if (c)
    for (int i = s.dfn[u]; i <= s.low[u]; ++i) Upd(s.seq[i], -1);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m >> str, CountM();

  is.fill(-1), it.fill(-1);
  for (int i = 0; i < n; ++i) {
    s.Append(str[i] - 'a');
    if (i >= m - 1) is[ns[i - m + 1] = s.pf] = i - m + 1;
  }
  for (int i = n - 1; ~i; --i) {
    t.Append(str[i] - 'a');
    if (i <= n - m) it[nt[i] = t.pf] = i;
  }
  s.Make(), t.Make(), Dfs(1), segt::Make(1, 1, t.d);
  t.fa[1] = 1;
  for (int i = 1; i <= t.d; ++i) anc[i][0] = t.fa[i];
  for (int i = 1; i < 20; ++i)
    for (int j = 1; j <= t.d; ++j) anc[j][i] = anc[anc[j][i - 1]][i - 1];
  Proc(1, false);

  for (int i = 0; i <= n - m; ++i) {
    int x = t.dfn[nt[i]], o = 1;
    while (segt::t[o].l - segt::t[o].r) {
      int m = (segt::t[o].l + segt::t[o].r) >> 1;
      segt::Push(o), o += o + (x > m);
    }
    ans[i] += segt::t[o].t;
  }

  for (int i = 0; i <= n - m; ++i) std::cout << ans[i] << "\n "[i < n - m];

  return 0;
}