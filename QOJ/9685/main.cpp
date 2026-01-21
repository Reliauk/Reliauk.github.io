#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E5 + 5, A = 62;

int n, m;
i64 all, out;
std::array<i64, N> a;
std::array<i64, A> ans;

std::array<int, N> ord;
std::array<std::vector<int>, N> opt;

std::array<bool, N> vis;
i64 Ans(int d, i64 exor) {
  i64 ans = 0;
  std::vector<int> tmp;
  for (int i = d; ~i; --i) {
    if (~exor >> i & 1) continue;
    for (int j : opt[i])
      if (!vis[j]) {
        i64 low = a[j] & ((i64(1) << i) - 1);
        tmp.push_back(j), vis[j] = true;
        ans += (i64(1) << i) - low, exor ^= (i64(1) << i) ^ low;
        break;
      }
  }
  for (int i : tmp) vis[i] = false;
  return exor + ans;
}

std::vector<std::map<int, i64>> plan;
i64 cur_ans, cur_xor;
std::map<int, i64> cur_plan;
std::array<std::set<int>, A> cur;
void Push(int x, int d) {
  i64 low = a[x] & ((i64(1) << d) - 1);
  cur[d].insert(x), vis[x] = true;
  cur_ans += (i64(1) << d) - low;
  cur_xor ^= (i64(1) << d) ^ low;
  cur_plan[x] += (i64(1) << d) - low;
  if (!cur_plan[x]) cur_plan.erase(x);
}
void Pop(int x, int d) {
  i64 low = a[x] & ((i64(1) << d) - 1);
  cur[d].erase(x), vis[x] = false;
  cur_ans -= (i64(1) << d) - low;
  cur_xor ^= (i64(1) << d) ^ low;
  cur_plan[x] -= (i64(1) << d) - low;
  if (!cur_plan[x]) cur_plan.erase(x);
}

int high;
std::array<bool, N> ban;
void Make(int d) {
  if (plan.size() >= m) return;
  if (d < 0) return void(plan.push_back(cur_plan));
  if (~cur_xor >> d & 1) return Make(d - 1);
  for (int i = d + 1; i < A; ++i)
    for (int x : cur[i]) {
      if (plan.size() >= m) return;
      cur_plan[x] += i64(1) << d, Make(d - 1);
      if (!(cur_plan[x] -= i64(1) << d)) cur_plan.erase(x);
    }
}
bool Dfs(int d) {
  if (plan.size() >= m) return false;
  if (cur_ans + Ans(d, cur_xor) > out) return false;
  if (d < 0) return Make(A - 1), true;
  if ((~cur_xor >> d & 1) || d - high) Dfs(d - 1);
  if (cur_xor >> d & 1)
    for (int i : opt[d])
      if (!vis[i]) {
        if (d == high && ban[i]) continue;
        if (plan.size() >= m) return true;
        Push(i, d);
        bool chk = Dfs(d - 1);
        if (Pop(i, d); !chk) break;
      }
  return true;
}

void Proc() {
  std::cin >> n >> m, all = 0;
  for (int h = 0; h < A; ++h) ans[h] = i64(1) << A;
  for (int i = 1; i <= n; ++i) std::cin >> a[i], all ^= a[i];
  if (!all) return void(std::cout << "0\n1\n0\n\n\n");

  for (int i = 1; i <= n; ++i) ord[i] = i;
  for (int d = 0; d < A; ++d) {
    int cnt = 0;
    static std::array<int, N> nxt;
    for (int i = 1; i <= n; ++i)
      if (a[ord[i]] >> d & 1) nxt[++cnt] = ord[i];
    opt[d].clear();
    for (int i = 1; i <= n; ++i)
      if (~a[ord[i]] >> d & 1) opt[d].push_back(nxt[++cnt] = ord[i]);
    for (int i = 1; i <= n; ++i) ord[i] = nxt[i];
  }

  for (int h = 0; h < A; ++h) {
    if (all >> (h + 1)) continue;
    if (opt[h].size() < 2 - (all >> h & 1)) continue;
    i64 low = a[opt[h][0]] & ((i64(1) << h) - 1);
    vis[opt[h][0]] = true;
    ans[h] = (i64(1) << h) - low + Ans(h, all ^ (i64(1) << h) ^ low);
    vis[opt[h][0]] = false;
  }
  out = *std::min_element(ALL(ans)), std::cout << out << '\n';

  plan.clear(), cur_ans = 0, cur_xor = all;
  for (int h = 0; h < A; ++h) {
    if (ans[h] - out) continue;
    high = h;
    for (int i : opt[h]) {
      if ((~all >> h & 1) && i == opt[h].back()) break;
      if (plan.size() >= m) break;
      Push(i, h);
      bool chk = Dfs(h);
      if (Pop(i, h); !chk) break;
      ban[i] = true;
    }
    for (int i : opt[h]) ban[i] = false;
  }

  std::cout << plan.size() << '\n';
  for (auto map : plan) {
    std::cout << map.size() << '\n';
    for (auto [x, _] : map) std::cout << x << ' ';
    std::cout << '\n';
    for (auto [_, y] : map) std::cout << y << ' ';
    std::cout << '\n';
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int c, t;
  std::cin >> c >> t;
  while (t--) Proc();

  return 0;
}