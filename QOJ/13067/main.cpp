#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5, MOD = 998244353;
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

using P = std::vector<int>;
P operator+(P a, P b) {
  if (a.size() < b.size()) return b + a;
  for (int i = 0; i < b.size(); ++i) Add(a[i], b[i]);
  return a;
}
void ShiftAdd(P &a, const P &b, int x) {
  if (a.size() < b.size() + x) a.resize(b.size() + x);
  for (int i = 0; i < b.size(); ++i) Add(a[i + x], b[i]);
}
std::array<int, 1 << 19> omg;
void MakeO() {
  for (int i = 1; i < (1 << 19); i += i) {
    int o = Power(3, (MOD - 1) / i / 2);
    for (int j = 0, m = 1; j < i; ++j, m = i64(m) * o % MOD) omg[i ^ j] = m;
  }
}
void Dft(int *it, int n) {
  static std::array<int, 1 << 19> inv;
  static std::array<u64, 1 << 19> tmp;
  tmp[0] = it[inv[0] = 0];
  for (int i = 1; i < n; ++i)
    tmp[i] = it[inv[i] = inv[i >> 1] >> 1 | (i & 1 ? n >> 1 : 0)];
  for (int i = 1; i < n; i += i)
    for (int j = 0; j < n; j += i + i)
      for (int k = 0; k < i; ++k) {
        const u64 x = tmp[j ^ i ^ k] * omg[i ^ k] % MOD;
        tmp[j ^ i ^ k] = tmp[j ^ k] + MOD - x, tmp[j ^ k] += x;
      }
  for (int i = 0; i < n; ++i) it[i] = tmp[i] % MOD;
}
void Idft(int *it, int n) {
  std::reverse(it + 1, it + n), Dft(it, n);
  for (int i = 0, inv = Inv(n); i < n; ++i) it[i] = i64(it[i]) * inv % MOD;
}
P operator*(P a, P b) {
  if (a.empty() || b.empty()) return {};
  if (a.size() < 32 || b.size() < 32) {
    P ans(a.size() + b.size() - 1);
    for (int i = 0; i < a.size(); ++i)
      for (int j = 0; j < b.size(); ++j) Add(ans[i + j], i64(a[i]) * b[j]);
    return ans;
  }
  static std::array<int, 1 << 19> f, g;
  int n = a.size() + b.size() - 1;
  if (n ^ (n & -n)) n = 2 << std::__lg(n);
  memset(data(f), 0, n * 4), memset(data(g), 0, n * 4);
  for (int i = 0; i < a.size(); ++i) f[i] = a[i];
  for (int i = 0; i < b.size(); ++i) g[i] = b[i];
  Dft(data(f), n), Dft(data(g), n);
  for (int i = 0; i < n; ++i) f[i] = i64(f[i]) * g[i] % MOD;
  return Idft(data(f), n), std::vector(PAR(f, 0, a.size() + b.size() - 1));
}

int n, q, d;
std::array<int, N> mid, ls, rs, ans;
void Make(int l, int r) {
  int o = ++d;
  if (l < mid[o]) ls[o] = d + 1, Make(l, mid[o]);
  if (mid[o] + 1 < r) rs[o] = d + 1, Make(mid[o] + 1, r);
}

std::array<int, N> dep, sz, hs;
void Dfs(int u) {
  sz[u] = 1, hs[u] = 0;
  for (int v : {ls[u], rs[u]}) {
    if (!v) continue;
    dep[v] = dep[u] + 1, Dfs(v), sz[u] += sz[v];
    if (sz[v] > sz[hs[u]]) hs[u] = v;
  }
}

std::array<int, N> sl, sr;
std::array<P, N> pl, pr, l, r;
void Proc(int u) {
  std::vector<int> ch{u};
  while (ch.back() < n) {
    Proc(ls[ch.back()] ^ rs[ch.back()] ^ hs[ch.back()]);
    ch.push_back(hs[ch.back()]);
  }
  reverse(ALL(ch)), pl[ch[0]] = pr[ch[0]] = {0, 1};
  for (int i = 1; i < ch.size(); ++i) {
    pl[ch[i]] = pr[ch[i]] = {0, 1, 0, MOD - 1};
    sl[i] = sl[i - 1] + 1, sr[i] = sr[i - 1] + 1;
    if (ls[ch[i]] == ch[i - 1]) {
      ++sr[i], ShiftAdd(pl[ch[i]], l[rs[ch[i]]], 2);
      ShiftAdd(pr[ch[i]], r[rs[ch[i]]], 1);
    } else {
      ++sl[i], ShiftAdd(pl[ch[i]], l[ls[ch[i]]], 1);
      ShiftAdd(pr[ch[i]], r[ls[ch[i]]], 2);
    }
  }
  auto proc = [&](auto self, int l, int r) {
    if (l == r) return;
    int m = (l + r) >> 1;
    self(self, l, m), self(self, m + 1, r);

    P ll, lr, rl, rr;
    for (int i = l; i <= m; ++i) {
      ShiftAdd(ll, pl[ch[i]], sl[m] - sl[i]);
      ShiftAdd(lr, pr[ch[i]], sr[m] - sr[i]);
    }
    for (int i = m + 1; i <= r; ++i)
      if (ls[ch[i]] == ch[i - 1])
        ShiftAdd(rl, ::l[rs[ch[i]]],
                 sr[i - 1] - sr[m] + dep[ch[i]] - dep[ch[r]]);
      else
        ShiftAdd(rr, ::r[ls[ch[i]]],
                 sl[i - 1] - sl[m] + dep[ch[i]] - dep[ch[r]]);
    ll = ll * rr, lr = lr * rl;
    for (int k = 0; k < ll.size(); ++k) Add(ans[k + dep[ch[r]]], ll[k]);
    for (int k = 0; k < lr.size(); ++k) Add(ans[k + dep[ch[r]]], lr[k]);
  };
  proc(proc, 0, ch.size() - 1), l[ch.back()] = r[ch.back()] = {};
  for (int i = 0; i < ch.size(); ++i) {
    ShiftAdd(l[ch.back()], pl[ch[i]], sl[ch.size() - 1] - sl[i]);
    ShiftAdd(r[ch.back()], pr[ch[i]], sr[ch.size() - 1] - sr[i]);
  }
}

void Proc() {
  for (int i = 1; i < n; ++i) std::cin >> mid[i];
  for (int i = 1; i < n + n; ++i) ls[i] = rs[i] = ans[i] = 0;
  d = 0, Make(1, n);
  for (int i = 1; i < n; ++i) {
    if (!ls[i]) ls[i] = ++d;
    if (!rs[i]) rs[i] = ++d;
  }

  dep[1] = 1, Dfs(1), Proc(1);

  for (int i = 1; i <= d; ++i) {
    Add(ans[dep[i]], 1);
    if (i < n) Add(ans[dep[i] + 2], MOD - 1);
  }
  for (int k; q; --q) std::cin >> k, std::cout << (k <= d ? ans[k] : 0) << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  MakeO();
  while (std::cin >> n >> q) Proc();

  return 0;
}