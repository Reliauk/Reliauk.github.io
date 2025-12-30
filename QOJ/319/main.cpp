#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 2E5 + 5;

int Border(std::string s) {
  int n = s.size();
  std::vector<int> pi(n, -1);
  for (int i = 1, j = -1; i < n; ++i) {
    while (j >= 0 && s[i] - s[j + 1]) j = pi[j];
    pi[i] = (j += (s[i] == s[j + 1]));
  }
  return pi.back() + 1;
}
std::string Map(std::string s) {
  if (s.empty()) return "";
  int n = s.size(), k = Border(s);
  if (k + k >= n) {
    int per = n - k;
    std::string t = Map(s.substr(n - per - n % per));
    reverse(ALL(t));
    for (int i = t.size(); i < n; ++i) t += t[i - per];
    return reverse(ALL(t)), t;
  }
  auto t = Map(s.substr(0, k));
  std::string r = t + std::string(n - k - k, '0') + t;
  if (Border(r) - k) r[r.size() - k - 1] = '1';
  return r;
}

void Proc() {
  std::string s;
  std::cin >> s;
  std::cout << Map(s) << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}