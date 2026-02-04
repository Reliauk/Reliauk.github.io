#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E3 + 5;
std::mt19937_64 rng(20090627);

int n, m;
std::array<std::array<int, N>, N> c;
int S(int u, int v, int w) { return c[u][v] == c[v][w] && c[u][v] >= 0; }

void Proc() {
  std::cin >> n >> m;
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j) c[i][j] = -1;
  std::vector<int> club;
  for (int i = 0, k; i < m; ++i) {
    std::cin >> k;
    std::vector<int> v(k);
    for (int &j : v) std::cin >> j;
    if (k > club.size()) club = v;
    for (int p : v)
      for (int q : v) c[p][q] = i;
  }

  if (n - club.size() < (club.size() + 1) / 2) return void(std::cout << "-1\n");

  std::vector<int> perm(n);
  static std::array<bool, N> in;
  for (int i = 1; i <= n; ++i) in[i] = false;
  for (int v : club) in[v] = true;
  for (int i = 0, p = 0, q = 1; i < n; ++i)
    if (i % 3 == 2 || p >= club.size()) {
      while (in[q]) ++q;
      perm[i] = q++;
    } else
      perm[i] = club[p++];

  auto calc = [&](int i) {
    return S(perm[i], perm[(i + 1) % n], perm[(i + 2) % n]);
  };
  int val = 0;
  for (int i = 0; i < n; ++i) val += calc(i);
  while (val) {
    int x = rng() % n, y = rng() % n, nv = val;
    std::set<int> ind{x,
                      y,
                      (x + n - 1) % n,
                      (x + n - 2) % n,
                      (y + n - 1) % n,
                      (y + n - 2) % n};
    for (int i : ind) nv -= calc(i);
    std::swap(perm[x], perm[y]);
    for (int i : ind) nv += calc(i);
    if (nv < val || (nv == val) && (rng() % 2))
      val = nv;
    else
      std::swap(perm[x], perm[y]);
  }

  for (int i : perm) std::cout << i << ' ';
  std::cout << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}