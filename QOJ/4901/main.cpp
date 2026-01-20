#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5;

int n, m;
i64 ans;
std::array<std::vector<int>, N> adj;

std::array<bool, N> win;
std::array<int, N> fa, dep;
std::unordered_set<i64> extra;
std::array<std::vector<int>, N> ext;
void FindFa(int u, int fa) {
  for (int v : adj[u])
    if (v - fa) ::fa[v] = u, dep[v] = dep[u] + 1, FindFa(v, u);
}
std::array<int, N> sw;
void Win(int u, int fa) {
  sw[u] = win[u];
  for (int v : adj[u])
    if (v - fa) Win(v, u), sw[u] += sw[v];
}
std::vector<std::pair<int, int>> sub;

struct V {
  int s;
  std::vector<int> v;
  void Reset() { s = 0, v.clear(); }
  int operator[](int x) const { return v[v.size() - x - 1]; }
  void operator+=(int x) { v.push_back(x), s += x; }
  void operator--() { s -= v.back(), v.pop_back(); }
};
void operator+=(V &a, V &b) {
  if (a.v.size() < b.v.size()) std::swap(a, b);
  for (int i = 0; i < b.v.size(); ++i) a.v[a.v.size() - i - 1] += b[i];
  a.s += b.s, b.Reset();
}
i64 Eval(const V &a, const V &b) {
  if (a.v.size() < b.v.size()) {
    i64 ans = i64(a.s) * b.s - Eval(b, a);
    for (int i = 0; i < a.v.size(); ++i) ans += i64(a[i]) * b[i];
    return ans;
  }
  i64 ans = 0;
  for (int i = 0, s = 0; i < b.v.size(); ++i) s += a[i], ans += i64(s) * b[i];
  return ans;
}
std::array<int, N> siz;
std::array<V, N> d, f, g;
void Dp(int u, int fa) {
  siz[u] = 1;
  for (int v : adj[u]) {
    if (v == fa) continue;
    bool jp = extra.count(i64(v) * n + fa);
    if (Dp(v, u); !jp) f[v] += 0;
    ans += Eval(f[v], d[u]) + Eval(g[v], d[u]);
    ans += Eval(f[u], d[v]) + Eval(g[u], d[v]);
    d[u] += d[v], (jp ? g : f)[u] += f[v], f[u] += g[v], siz[u] += siz[v];
  }
  if (f[u].v.empty()) f[u] += 0;
  d[u] += 1, g[u] += 0, f[u].v.back() = 1, ++f[u].s;
  ans += siz[u] - 1;
}

int key, ff, uu;
std::array<int, N> dis, to;
std::array<std::vector<int>, N> rad;
std::array<std::vector<std::pair<int, int>>, N> subr;
void Subr(int u, int r, int w) { subr[r + dep[u]].emplace_back(u, w); }
void Dfs(int u, int fa) {
  if (to[u] == ff || key <= 0) {
    int lim = dis[u] + (to[u] != ff && key < 0) + 1;
    if (uu == ::fa[ff])
      Subr(ff, lim, 1);
    else
      rad[ff].push_back(lim), Subr(uu, lim - 1, -1);
  } else {
    int lim = dis[u] + 1;
    if (key == ::fa[ff]) {
      rad[ff].push_back(lim + 1), Subr(ff, lim + 1, -1);
      Subr(ff, lim, 1), Subr(uu, lim - 1, -1);
    } else if (uu == ::fa[ff])
      Subr(ff, lim, 1), Subr(key, lim, 1), Subr(key, lim - 1, -1);
    else {
      rad[ff].push_back(lim), Subr(uu, lim - 1, -1);
      Subr(key, lim, 1), Subr(key, lim - 1, -1);
    }
  }
  for (int v : adj[u]) {
    if (v == fa) continue;
    int up = extra.count(i64(v) * n + fa) ? fa : u;
    dis[v] = dis[up] + 1, to[v] = to[up], Dfs(v, u);
  }
}

int dt;
std::array<int, N> dfn, low;
std::array<std::vector<int>, N> depp;
void Mark(int u, int fa) {
  dfn[u] = ++dt, depp[dep[u]].push_back(dfn[u]);
  for (int v : adj[u])
    if (v - fa) Mark(v, u);
  low[u] = dt;
}
std::array<int, N> bit;
void Add(int x, int v) {
  for (; x <= n; x += x & -x) bit[x] += v;
}
int Query(int x) {
  int v = 0;
  for (; x; x &= x - 1) v += bit[x];
  return v;
}
int Query(int l, int r) { return Query(r) - Query(l - 1); }

std::array<bool, N> ers;
int rt, all;
std::array<int, N> mx;
void Find(int u, int fa) {
  siz[u] = 1, mx[u] = 0;
  for (int v : adj[u])
    if (v - fa && !ers[v])
      Find(v, u), siz[u] += siz[v], mx[u] = std::max(mx[u], siz[v]);
  mx[u] = std::max(mx[u], all - siz[u]);
  if (mx[u] < mx[rt]) rt = u;
}
int Get(int u, int s) { return mx[rt = 0] = all = s, Find(u, 0), rt; }
std::array<int, N> curd;
std::vector<int> cnt, set;
void Pull(int u, int fa) {
  curd[u] = curd[fa] + 1, set.push_back(u);
  if (cnt.size() <= curd[u]) cnt.resize(curd[u] + 1);
  ++cnt[curd[u]];
  for (int v : adj[u])
    if (v - fa && !ers[v]) Pull(v, u);
}
void Eval(int u) {
  cnt.clear(), set.clear(), curd[0] = -1, Pull(u, 0);
  for (int i = cnt.size() - 1; i; --i) cnt[i - 1] += cnt[i];
  for (int u : set)
    for (int r : rad[u]) {
      r = std::max(r - curd[u], 0);
      if (r < cnt.size()) ans += cnt[r];
    }
  for (int v : adj[u]) {
    if (ers[v]) continue;
    cnt.clear(), set.clear(), Pull(v, u);
    for (int i = cnt.size() - 1; i; --i) cnt[i - 1] += cnt[i];
    for (int u : set)
      for (int r : rad[u]) {
        r = std::max(r - curd[u], 0);
        if (r < cnt.size()) ans -= cnt[r];
      }
  }
}
void Proc(int u) {
  ers[u] = true, Eval(u), Find(u, 0);
  for (int v : adj[u])
    if (!ers[v]) Proc(Get(v, siz[v]));
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int i = 1, u, v; i < n; ++i)
    std::cin >> u >> v, adj[u].push_back(v), adj[v].push_back(u);

  FindFa(1, 0);
  for (int u, v; m; --m) {
    std::cin >> u >> v;
    if (fa[u] == v || fa[v] == u) continue;
    if (fa[u] == fa[v] || fa[fa[u]] == v || fa[fa[v]] == u) {
      extra.emplace(i64(u) * n + v), extra.emplace(i64(v) * n + u);
      ext[u].push_back(v), ext[v].push_back(u);
    } else
      win[u] = win[v] = true;
  }
  Win(1, 0);
  for (int i = 1; i <= n; ++i) {
    win[i] |= sw[i] && sw[i] != sw[1];
    int cnt = 0;
    for (int v : adj[i])
      if (v - fa[i]) cnt += !!sw[v];
    win[i] |= cnt > 1;
  }
  for (int f = 1; f <= n; ++f)
    for (int u : adj[f])
      if (win[f] && !win[u]) sub.emplace_back(f, u);
  ans = i64(n - 1) * std::count(ALL(win), true);
  if (!ans) std::cout << "0\n", exit(0);

  for (auto [f, u] : sub) Dp(u, f);

  for (auto [f, u] : sub) {
    std::vector<int> out;
    for (int g : ext[u])
      if (win[g]) out.push_back(g);
    key = out.empty() ? -1 : out.size() > 1 ? 0 : out[0];
    ff = to[f] = f, uu = to[u] = u, Dfs(u, f);
  }

  Mark(1, 0);
  for (int i = n; ~i; --i) {
    for (int v : depp[i]) Add(v, 1);
    for (auto [u, w] : subr[i]) ans += w * Query(dfn[u], low[u]);
  }

  Proc(Get(1, n));

  std::cout << ans << '\n';

  return 0;
}