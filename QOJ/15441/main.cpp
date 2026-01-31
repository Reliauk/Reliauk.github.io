#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5;

int n;
i64 t;
std::array<int, 3> l, p;
std::array<std::vector<int>, 3> len;

struct S {
  int sc, ss;
  i64 ans;
  std::array<int, 301> cnt;
  void Reset() { cnt.fill(0), sc = 0, ss = 0; }
  void Add(int x, int v) {
    for (int i = 1; i < x; ++i) ans += i * cnt[i] * v;
    cnt[x] += v, sc += v, ss += x * v;
    for (int i = x; i < 301; ++i) ans += x * cnt[i] * v;
    if (v < 0) ans -= x;
  }
} lp, mp, rp;

void Proc() {
  std::cin >> n >> l[0] >> l[1] >> l[2];
  p[0] = 0, p[1] = 1, p[2] = 2;
  if (l[p[0]] < l[p[1]]) std::swap(p[0], p[1]);
  if (l[p[1]] < l[p[2]]) std::swap(p[1], p[2]);
  if (l[p[0]] < l[p[1]]) std::swap(p[0], p[1]);
  assert(l[p[0]] >= l[p[1]] && l[p[1]] >= l[p[2]]);

  int sum = 0;
  for (int i : {0, 1, 2}) len[i].clear();
  for (int i = 0, p, c; i < n; ++i)
    std::cin >> p >> c, len[--p].push_back(c), sum += c;
  std::cin >> t;
  for (int i : {0, 1, 2}) std::sort(ALL(len[i]), std::greater());
  if (l[p[0]] < sum) return void(std::cout << "NO\n");

  lp.Reset(), mp.Reset(), rp.Reset();
  auto ans = [&]() {
    i64 ans = lp.ans + mp.ans + rp.ans;
    ans += i64(mp.sc + rp.sc) * lp.ss + i64(rp.sc) * mp.ss;
    return ans;
  };

  std::multiset<int> ce;
  std::multiset<int, std::greater<int>> cd;
  for (int c : len[p[1]]) lp.Add(c, 1), cd.insert(c);
  for (int c : len[p[2]]) lp.Add(c, 1);
  for (int c : len[p[0]]) rp.Add(c, 1);
  if (lp.ss > l[p[1]]) return void(std::cout << "NO\n");
  while (cd.size() && lp.ss > l[p[2]])
    lp.Add(*begin(cd), -1), mp.Add(*begin(cd), 1), ce.insert(*begin(cd)),
        cd.erase(begin(cd));
  if (lp.ss > l[p[2]]) return void(std::cout << "NO\n");
  if (ans() < t) return void(std::cout << "YES\n");

  for (int i = int(len[p[0]].size()) - 1; ~i; --i) {
    int cur = len[p[0]][i];
    if (lp.ss + mp.ss + cur > l[p[1]]) break;
    rp.Add(cur, -1);
    if ((cd.size() && *begin(cd) < cur) ||
        (cd.empty() && ce.size() && cur >= *begin(ce)))
      mp.Add(cur, 1), ce.insert(cur);
    else
      lp.Add(cur, 1), cd.insert(cur);
    while (cd.size() && lp.ss > l[p[2]])
      lp.Add(*begin(cd), -1), mp.Add(*begin(cd), 1), ce.insert(*begin(cd)),
          cd.erase(begin(cd));
    if (lp.ss > l[p[2]]) continue;
    while (ce.size() && lp.ss + *begin(ce) <= l[p[2]])
      mp.Add(*begin(ce), -1), lp.Add(*begin(ce), 1), cd.insert(*begin(ce)),
          ce.erase(begin(ce));
    if (ans() < t) return void(std::cout << "YES\n");
  }

  std::cout << "NO\n";
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}