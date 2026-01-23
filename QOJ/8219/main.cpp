#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5, MOD = 998244353;
std::mt19937_64 rng(20090627);
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

int n, m, d;
std::array<int, N> mid, lc, rc;
int Make(int l, int r) {
  if (l + 1 == r) return n + l;
  int cur = ++d;
  lc[cur] = Make(l, mid[cur]);
  rc[cur] = Make(mid[cur], r);
  return cur;
}

std::map<u64, int> id;
std::array<u64, N> w, val;
std::array<int, N> col, l, r;

std::array<int, N * 2> g, sf, mf;
std::array<std::unordered_map<int, int>, N * 2> f;
void Dp(int u) {
  if (u >= n) return sf[u] = mf[u] = 1, void(g[u] = f[u][col[u - n]] = 1);
  Dp(lc[u]), Dp(rc[u]);
  if (f[lc[u]].size() < f[rc[u]].size()) std::swap(lc[u], rc[u]);

  f[u].swap(f[lc[u]]), g[u] = g[lc[u]], sf[u] = sf[lc[u]], mf[u] = mf[lc[u]];
  if (g[rc[u]]) {
    int inv = Inv(g[rc[u]]);
    for (auto [i, v] : f[rc[u]]) {
      int add = i64(f[u][i] + g[u]) * inv % MOD * v % MOD;
      Add(f[u][i], add), Add(sf[u], add);
    }
    Add(g[u], Sum(g[u], sf[u]));
    mf[u] = i64(mf[u]) * mf[rc[u]] % MOD * g[rc[u]] % MOD;
  } else {
    sf[u] = 0;
    for (auto &[i, v] : f[rc[u]])
      Add(sf[u], v = i64(f[u][i] + g[u]) * v % MOD * mf[rc[u]] % MOD);
    f[u].swap(f[rc[u]]), g[u] = sf[u];
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int i = 1; i < n; ++i) std::cin >> mid[i];
  Make(0, n);

  for (int i = 0; i < m; ++i) {
    std::cin >> l[i] >> r[i], w[i] = rng();
    val[l[i]] ^= w[i], val[r[i]] ^= w[i];
  }
  for (int i = 0; i < n; ++i) {
    if (!id.count(val[i])) id[val[i]] = id.size() + 1;
    col[i] = id[val[i]], val[i + 1] ^= val[i];
  }

  Dp(1);
  int ans = g[1];
  if (id.count(0)) Add(ans, f[1][id[0]]);
  std::cout << i64(ans) * mf[1] % MOD << '\n';

  return 0;
}