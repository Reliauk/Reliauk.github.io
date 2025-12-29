#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using P = std::vector<i64>;
P operator+(P a, P b) {
  if (a.size() > b.size()) std::swap(a, b);
  for (int i = 0; i < a.size(); ++i) b[i] = std::max(b[i], a[i]);
  return b;
}
P operator*(const P &a, const P &b) {
  if (a.empty() || b.empty()) return {};
  P p(a.size() - 1), q(b.size() - 1), r(p.size() + q.size() + 1);
  for (int i = 1; i < a.size(); ++i) p[i - 1] = a[i] - a[i - 1];
  for (int i = 1; i < b.size(); ++i) q[i - 1] = b[i] - b[i - 1];
  merge(ALL(p), ALL(q), begin(r) + 1, std::greater());
  return r[0] = a[0] + b[0], partial_sum(ALL(r), begin(r)), r;
}

const int N = 2E5 + 5;
const i64 INF = 1E18;

int n;
std::array<std::vector<std::pair<int, int>>, N> adj;

std::array<int, N> siz, son;
void Dfs(int u, int fa) {
  siz[u] = 1;
  for (auto [v, _] : adj[u]) {
    if (v == fa) continue;
    Dfs(v, u), siz[u] += siz[v];
    if (siz[v] > siz[son[u]]) son[u] = v;
  }
}

std::array<P, N> f, g;
void Proc(int u, int fa) {
  std::vector<std::array<std::array<P, 2>, 2>> mat;
  for (int cur = u, lst = fa; son[cur]; lst = cur, cur = son[cur]) {
    std::vector<std::pair<P, P>> mg;
    for (auto [v, w] : adj[cur])
      if (v - lst && v - son[cur])
        Proc(v, cur), mg.emplace_back(f[v] + g[v], f[v] * P{-INF, w});
    auto calc = [&](auto self, int l, int r) {
      if (l == r) return std::pair{P{0}, P{}};
      if (l + 1 == r) return mg[l];
      int m = (l + r) >> 1;
      auto [a, b] = self(self, l, m);
      auto [c, d] = self(self, m, r);
      return std::pair{a * c, a * d + b * c};
    };
    auto [z, o] = calc(calc, 0, mg.size());
    std::array<std::array<P, 2>, 2> tr;
    int wt = 0;
    for (auto [v, w] : adj[cur])
      if (v == son[cur]) wt = w;
    tr[0][0] = z, tr[0][1] = o + (z * P{-INF, wt});
    tr[1][0] = z, tr[1][1] = o;
    mat.push_back(tr);
  }
  reverse(ALL(mat));

  auto calc = [&](auto self, int l, int r) {
    if (l + 1 == r) return mat[l];
    int m = (l + r) >> 1;
    auto lp = self(self, l, m), rp = self(self, m, r);
    std::array<std::array<P, 2>, 2> ans;
    for (int a : {0, 1})
      for (int b : {0, 1})
        for (int c : {0, 1}) ans[a][b] = ans[a][b] + lp[a][c] * rp[c][b];
    return ans;
  };
  if (mat.size()) {
    auto mul = calc(calc, 0, mat.size());
    f[u] = mul[0][0], g[u] = mul[0][1];
  } else
    f[u] = {0};
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;
  for (int i = 1, u, v, w; i < n; ++i) {
    std::cin >> u >> v >> w;
    adj[u].emplace_back(v, w);
    adj[v].emplace_back(u, w);
  }

  Dfs(1, 0), Proc(1, 0);
  P ans = f[1] + g[1];
  for (int i = 1; i < ans.size(); ++i) std::cout << ans[i] << ' ';
  for (int i = ans.size(); i < n; ++i) std::cout << "? ";
  std::cout << '\n';

  return 0;
}