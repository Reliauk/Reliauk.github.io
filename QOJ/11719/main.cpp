#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5;
i64 C(int n) { return i64(n + 1) * n / 2; }

int n, m;
i64 k, ans;
int ar, br, as, bs;
int mr, cr, ms, cs;
std::array<int, N> r, s, pr, ps;
std::array<std::vector<int>, N> misc;

namespace ds {
i64 ans;
std::multiset<int> s;
void Push(int x) {
  auto it = s.insert(x);
  int l = *prev(it);
  int r = *next(it);
  ans += C(x - l - 1) + C(r - x - 1) - C(r - l - 1);
}
void Pop(int x) {
  auto it = s.find(x);
  int l = *prev(it);
  int r = *next(it);
  s.erase(it);
  ans -= C(x - l - 1) + C(r - x - 1) - C(r - l - 1);
}
void Reset() { s = {-1, m}, ans = C(m); }
void Calc(int id) {
  for (int y : misc[id]) Push(y);
  ::ans += ans;
  for (int y : misc[id]) Pop(y);
}
}  // namespace ds

std::array<bool, N> vis;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m >> k;
  std::cin >> r[1] >> ar >> br;
  std::cin >> s[1] >> as >> bs;

  std::map<int, int> occ;
  for (int i = 2;; ++i) {
    r[i] = (i64(r[i - 1]) * ar + br) % n;
    if (occ.count(r[i])) {
      mr = occ[r[i]] - 1, cr = i - occ[r[i]];
      break;
    }
    occ[r[i]] = i;
  }
  occ.clear();
  for (int i = 2;; ++i) {
    s[i] = (i64(s[i - 1]) * as + bs) % m;
    if (occ.count(s[i])) {
      ms = occ[s[i]] - 1, cs = i - occ[s[i]];
      break;
    }
    occ[s[i]] = i;
  }

  int lim = std::min<i64>(k, std::max(mr, ms)) + 1;
  k -= lim - 1;
  for (int i = mr + 1; i <= mr + cr; ++i) pr[((i - lim) % cr + cr) % cr] = r[i];
  for (int i = ms + 1; i <= ms + cs; ++i) ps[((i - lim) % cs + cs) % cs] = s[i];
  for (int i = 1; i < lim; ++i) {
    int x = i <= mr ? r[i] : pr[((i - lim) % cr + cr) % cr];
    int y = i <= ms ? s[i] : ps[((i - lim) % cs + cs) % cs];
    misc[x].emplace_back(y);
  }

  int cnt = k / cr;
  for (int i = 0; i < cs; ++i) {
    if (vis[i]) continue;
    std::vector<int> circ;
    for (int j = i; !vis[j]; j = (j + cr) % cs)
      circ.push_back(j), vis[j] = true;
    ds::Reset();
    for (int j = 0; j < cnt; ++j) ds::Push(ps[circ[j]]);
    for (int j = 0; j < circ.size(); ++j) {
      if (circ[j] < cr) {
        i64 x = circ[j] + i64(cnt) * cr, mx = cnt - 1;
        if (x < k) ds::Push(ps[x % cs]), ++mx;
        ds::Calc(pr[circ[j]]);
        i64 pp = mx;
        for (int k = circ[j] + cs; k < cr; k += cs) {
          while (pp >= 0 && pp * cr + k >= ::k)
            ds::Pop(ps[(pp-- * cr + k) % cs]);
          ds::Calc(pr[k]);
        }
        while (pp < mx) ds::Push(ps[(++pp * cr + circ[j]) % cs]);
        if (x < k) ds::Pop(ps[x % cs]);
      }
      if (cnt) {
        ds::Pop(ps[circ[j]]);
        ds::Push(ps[circ[(j + cnt) % circ.size()]]);
      }
    }
  }

  occ.clear(), ds::Reset();
  for (int i = 0; i < cr; ++i) occ[pr[i]] = 1;
  for (int i = 0; i < n; ++i)
    if (!occ.count(i)) ds::Calc(i);
  std::cout << ans << '\n';

  return 0;
}