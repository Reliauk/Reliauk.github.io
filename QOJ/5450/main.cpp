#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 405;
std::mt19937_64 rng(20090627);

int n, m;
std::array<std::vector<int>, N> club;
std::array<std::array<std::array<bool, N>, N>, N> vis;

void Proc() {
  std::cin >> n >> m;
  for (int i = 1; i <= n; ++i) club[i].clear();
  bool no = false;
  for (int i = 0, k; i < m; ++i) {
    std::cin >> k;
    for (int j = 0, x; j < k; ++j) std::cin >> x, club[x].push_back(i);
    if ((k + 1) / 2 > (n - k)) no = true;
  }
  if (no) return void(std::cout << "-1\n");
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      std::vector<int> cnt(m);
      for (int x : club[i]) ++cnt[x];
      for (int x : club[j]) ++cnt[x];
      for (int k = 1; k <= n; ++k) {
        bool suc = false;
        for (int x : club[k]) {
          suc |= cnt[x] == 2;
          if (suc) break;
        }
        vis[i][j][k] = !suc;
      }
    }
  }

  std::vector<int> perm(n);
  std::iota(ALL(perm), 1), std::shuffle(ALL(perm), rng);
  int ok = 0;
  auto val = [&](int i) -> int {
    i %= n;
    int j = (i + n - 2) % n, k = (i + n - 1) % n;
    return vis[perm[i]][perm[j]][perm[k]];
  };
  for (int i = 0; i < n; ++i) ok += val(i);
  while (ok - n) {
    int i = (rng() % (n - 1) + n - 1) % (n - 1);
    int nok = ok;
    nok -= val(i) + val(i + 1) + val(i + 2) + val(i + 3);
    std::swap(perm[i], perm[i + 1]);
    nok += val(i) + val(i + 1) + val(i + 2) + val(i + 3);
    if (nok > ok || (rng() & 1)) {
      ok = nok;
      continue;
    }
    std::swap(perm[i], perm[i + 1]);
  }
  for (int i : perm) std::cout << i << ' ';
  return void(std::cout << '\n');
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}