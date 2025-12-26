#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 5E5 + 5;
std::mt19937 rng(20090627);

int dbg;

int n;
std::vector<std::string> row;
std::string Ask(std::string s) {
  auto query = [&]() {
    int r;
    ++dbg;
    std::cout << "? 1 " << s.size() << '\n' << s << '\n', std::cout.flush();
    return std::cin >> r, r;
  };
  if (!query()) return "";
  while (s.size() < n) {
    char c = (rng() & 1) + '0';
    if (s += c; query()) continue;
    if (s.back() = c ^ 1; query()) continue;
    s.pop_back();
    break;
  }
  while (s.size() < n) {
    char c = (rng() & 1) + '0';
    if (s = c + s; !query()) s[0] = c ^ 1;
  }
  return s;
}

int d;
std::vector<int> rts;
std::array<int, N> fa;
std::array<std::array<int, 2>, N> to;
void Add(std::string s) {
  int u = 0;
  for (char c : s) {
    if (!to[u][c - '0']) fa[to[u][c - '0'] = ++d] = u;
    u = to[u][c - '0'];
  }
}
void Push(std::string s) {
  for (int i = 0; i < s.size(); ++i) Add(s.substr(i));
}
bool Exist(std::string s) {
  int u = 0;
  for (char c : s)
    if ((u = to[u][c - '0']) < 0) return false;
  return true;
}
std::string Get(int u) {
  std::string ans;
  for (; u; u = fa[u]) ans += (u == to[fa[u]][1]) + '0';
  return reverse(ALL(ans)), ans;
}

std::vector<int> cur{0};
bool Query() {
  int r;
  ++dbg;
  std::cout << "? " << cur.size() << ' ' << n << '\n';
  for (int i : cur) std::cout << row[i] << '\n';
  std::cout.flush();
  return std::cin >> r, r;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;

  rts = {0};
  for (int it = 1; rts.size() && it <= n; ++it) {
    for (int u : rts)
      for (int o : {0, 1}) {
        if (to[u][o]) continue;
        auto str = Get(u) += (o + '0');
        bool suc = true;
        for (int i = 1; suc && i < str.size(); ++i) suc &= Exist(str.substr(i));
        if (!suc) continue;
        if (str = Ask(str); str.size())
          row.push_back(str), Push(str);
        else
          to[u][o] = -1;
      }
    std::vector<int> nxt;
    for (int u : rts)
      for (int o : {0, 1})
        if (to[u][o] > 0) nxt.push_back(to[u][o]);
    rts.swap(nxt);
  }

  while (cur.size() < n) {
    bool fd = false;
    for (int i = 0; !fd && i < row.size(); ++i) {
      cur.push_back(i);
      if (Query()) {
        fd = true;
        continue;
      }
      cur.pop_back();
    }
    if (!fd) break;
  }
  while (cur.size() < n)
    for (int i = 0; cur.size() < n && i < row.size(); ++i) {
      cur.insert(begin(cur), i);
      if (Query()) continue;
      cur.erase(begin(cur));
    }

  std::cout << "!\n";
  for (int i : cur) std::cout << row[i] << '\n';
  std::cout.flush();

  std::cerr << dbg << "HH\n";

  return 0;
}