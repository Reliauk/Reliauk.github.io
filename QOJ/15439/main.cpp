#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

std::string s;

void Proc() {
  std::cin >> s;
  for (int i = 1; i < s.size(); ++i)
    if (s[i - 1] == s[i]) return void(std::cout << s.size() - 1 << '\n');
  std::cout << s.size() - 2 << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}