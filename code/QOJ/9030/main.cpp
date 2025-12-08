#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 5E4 + 5, M = 5;
const i64 INF = 1E18;

int n, m;
std::array<int, N * M> a, b, le, up, val;

std::array<int, N * M> cur;
std::array<std::array<i64, 1 << M>, N * M> f;
std::array<std::array<int, 1 << M>, N * M> g;
void Proc(std::vector<int> v, int l, int r) {
  if (v.empty() || l == r) return;
  int vm = (l + r) >> 1;
  for (int i = 0; i < v.size(); ++i) cur[v[i]] = i;
  for (int i = 0; i < v.size(); ++i) {
    for (int s = 0; s < 1 << m; ++s) f[i][s] = INF;
    for (int s = 0; s < 1 << (m + 1); ++s) {
      i64 w = i ? f[i - 1][s & ~(1 << m)] : 0;
      int o = s >> m & 1;
      if (!o && a[v[i]] > vm || o && b[v[i]] <= vm) continue;
      if (v[i] % m) {
        int p = ~cur[v[i] - 1] ? s >> (m - 1) & 1 : val[v[i] - 1] > vm;
        w += (o ^ p) * le[v[i]];
      }
      if (v[i] >= m) {
        int sh = i - cur[v[i] - m];
        int p = ~cur[v[i] - m] ? s >> (m - sh) & 1 : val[v[i] - m] > vm;
        w += (o ^ p) * up[v[i]];
      }
      if (v[i] % m + 1 < m && !~cur[v[i] + 1])
        w += (o ^ (val[v[i] + 1] > vm)) * le[v[i] + 1];
      if (v[i] + m < n * m && !~cur[v[i] + m])
        w += (o ^ (val[v[i] + m] > vm)) * up[v[i] + m];
      if (w < f[i][s >> 1]) f[i][s >> 1] = w, g[i][s >> 1] = s & ~(1 << m);
    }
  }
  for (int i : v) cur[i] = -1;

  std::vector<int> lv, rv;
  int s = std::min_element(PAR(f[v.size() - 1], 0, (1 << m))) -
          data(f[v.size() - 1]);
  for (int i = v.size() - 1; ~i; s = g[i--][s])
    (s >> (m - 1) & 1 ? rv : lv).push_back(v[i]);
  reverse(ALL(lv)), reverse(ALL(rv));
  for (int i : rv) val[i] = std::max(val[i], vm + 1);
  Proc(lv, l, vm), Proc(rv, vm + 1, r);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m;
  for (int i = 0; i < m; ++i)
    for (int j = i; j < n * m; j += m) std::cin >> a[j] >> b[j];
  for (int i = 0; i < m; ++i)
    for (int j = i + m; j < n * m; j += m) std::cin >> up[j];
  for (int i = 1; i < m; ++i)
    for (int j = i; j < n * m; j += m) std::cin >> le[j];

  std::vector<int> u(n * m);
  iota(ALL(u), 0), cur.fill(-1), Proc(u, 0, 1E4);

  i64 ans = 0;
  for (int i = 0; i < n * m; ++i) {
    if (i >= m) ans += std::abs(val[i] - val[i - m]) * up[i];
    if (i % m) ans += std::abs(val[i] - val[i - 1]) * le[i];
  }
  std::cout << ans << '\n';

  return 0;
}