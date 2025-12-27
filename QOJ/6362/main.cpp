#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5, MOD = 1E9 + 7;
void Add(int &x, i64 y) { x = (x + y) % MOD; }
void Add(int &x, int y) {
  if ((x += y) >= MOD) x -= MOD;
}
int Sum(int x, int y) { return Add(x, y), x; }
int Power(int a, int b) {
  int r = 1;
  for (; b; b >>= 1, a = i64(a) * a % MOD)
    if (b & 1) r = i64(r) * a % MOD;
  return r;
}
int Inv(int a) { return Power(a, MOD - 2); }
std::array<int, N> fac, ifac;
int Binom(int x, int y) {
  if (x < 0 || y < 0 || x < y) return 0;
  return i64(fac[x]) * ifac[y] % MOD * ifac[x - y] % MOD;
}
void Math() {
  for (int i = fac[0] = 1; i < N; ++i) fac[i] = i64(fac[i - 1]) * i % MOD;
  ifac.back() = Inv(fac.back());
  for (int i = N - 1; i; --i) ifac[i - 1] = i64(ifac[i]) * i % MOD;
}

int n, k, r, ans;
std::array<int, N> rad, cnt;
std::array<std::vector<std::pair<int, int>>, N> adj;

std::array<bool, N> ers;
int rt, all;
std::array<int, N> sz, mx;
void Find(int u, int fa) {
  sz[u] = 1, mx[u] = 0;
  for (auto [v, _] : adj[u])
    if (v - fa && !ers[v])
      Find(v, u), sz[u] += sz[v], mx[u] = std::max(mx[u], sz[v]);
  mx[u] = std::max(mx[u], all - sz[u]);
  if (mx[u] < mx[rt]) rt = u;
}
int Get(int u, int s) { return mx[rt = 0] = all = s, Find(u, 0), rt; }
std::vector<int> cur;
std::array<i64, N> dep;
void Dfs(int u, int fa) {
  cur.push_back(u);
  for (auto [v, w] : adj[u])
    if (v - fa && !ers[v]) dep[v] = dep[u] + w, Dfs(v, u);
}
std::vector<i64> raw;
std::array<int, N> bit;
void AddB(int x, int v) {
  for (++x; x < N; x += x & -x) bit[x] += v;
}
int Query(int x) {
  int v = 0;
  for (++x; x; x &= x - 1) v += bit[x];
  return v;
}
void Eval(int u) {
  cur.clear(), dep[u] = 0, Dfs(u, 0), raw.clear();
  for (int i : cur) raw.push_back(dep[i]);
  sort(ALL(raw)), raw.erase(unique(ALL(raw)), end(raw));
  auto ccur = cur;
  for (int i : cur)
    if (i <= n) AddB(std::lower_bound(ALL(raw), dep[i]) - begin(raw), 1);
  auto eval = [&](const std::vector<int> &v) {
    for (int i : v)
      if (i <= n) AddB(std::lower_bound(ALL(raw), dep[i]) - begin(raw), -1);
    for (int i : v) {
      int it = std::upper_bound(ALL(raw), rad[i] - dep[i]) - begin(raw);
      cnt[i] += Query(it - 1);
    }
    for (int i : v)
      if (i <= n) AddB(std::lower_bound(ALL(raw), dep[i]) - begin(raw), 1);
  };
  eval({u});
  for (auto [v, w] : adj[u])
    if (!ers[v]) cur.clear(), Dfs(v, u), eval(cur);
  for (int i : ccur)
    if (i <= n) AddB(std::lower_bound(ALL(raw), dep[i]) - begin(raw), -1);
}
void Proc(int u) {
  ers[u] = true, Find(u, 0), Eval(u);
  for (auto [v, _] : adj[u])
    if (!ers[v]) Proc(Get(v, sz[v]));
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  Math();

  std::cin >> n >> k >> r;
  for (int i = 1, u, v, w; i < n; ++i) {
    std::cin >> u >> v >> w;
    adj[u].emplace_back(i + n, w);
    adj[v].emplace_back(i + n, w);
    adj[i + n] = {{u, w}, {v, w}};
  }
  for (int i = 1; i <= n; ++i) rad[i] = r + r;
  for (int i = n + 1; i < n + n; ++i) rad[i] = r + r - adj[i][0].second;

  Proc(Get(1, n));

  for (int i = 1; i <= n; ++i) ++cnt[i];

  for (int i = 1; i <= n; ++i) Add(ans, Binom(cnt[i], k));
  for (int i = n + 1; i < n + n; ++i) Add(ans, MOD - Binom(cnt[i], k));
  for (int i = 1; i <= k; ++i) ans = i64(ans) * i % MOD;
  std::cout << ans << '\n';

  return 0;
}