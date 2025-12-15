#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using f64 = double;

const int N = 2E3 + 5;
std::mt19937_64 rng(20090627);
f64 Rand() { return std::uniform_real_distribution<f64>(0, 1)(rng); }
std::array<std::array<bool, N>, N> cop;
void Math() {
  cop[0][1] = true;
  for (int a = 1; a < N; ++a)
    for (int b = a; b < N; ++b) cop[b][a] = cop[a][b] = cop[b % a][a];
}

int n, m;
std::array<int, 4> l;

std::array<int, N> ans, prv, nxt;
int Eval() {
  int res = 0;
  for (int i = 0, s = 0; i < m; s += l[i++]) {
    if (l[i] == 1) continue;
    for (int j = s + 1; j < s + l[i]; ++j)
      res += std::gcd(ans[j], ans[j + 1]) != 1;
    res += std::gcd(ans[s + 1], ans[s + l[i]]) != 1;
  }
  return res;
}

void Proc() {
  std::string s;
  std::getline(std::cin, s), n = m = 0;
  if (s.empty()) std::getline(std::cin, s);
  while (s.size()) {
    int p = 0;
    while (p < s.size() && isdigit(s[p])) ++p;
    n += (l[m++] = std::stoi(s.substr(0, p)));
    s = s.substr(std::min<int>(s.size(), p + 1));
  }

  int chk = n / 2;
  for (int i = 0; i < m; ++i) chk -= std::max(1, l[i] / 2);
  if (chk > 0) return void(std::cout << "QQ\n");
  std::cout << "^_<\n";

  for (int i = 0, s = 0; i < m; s += l[i++])
    for (int j = s + 1; j <= s + l[i]; ++j) {
      prv[j] = (j == s + 1 ? s + l[i] : j - 1);
      nxt[j] = (j == s + l[i] ? s + 1 : j + 1);
    }
  while (true) {
    for (int i = 1; i <= n; ++i) ans[i] = i;
    int val = Eval();
    for (f64 t = 0.1; val && t > 0.002; t *= 0.9999) {
      int x = rng() % n + 1, y = rng() % (n - 1) + 1;
      y += (y >= x);
      int nxt = val;
      if (x - prv[x]) {
        nxt -= !cop[ans[x]][ans[prv[x]]];
        nxt -= !cop[ans[x]][ans[::nxt[x]]];
      }
      if (y - prv[y]) {
        if (prv[y] - x) nxt -= !cop[ans[y]][ans[prv[y]]];
        if (::nxt[y] - x) nxt -= !cop[ans[y]][ans[::nxt[y]]];
      }

      std::swap(ans[x], ans[y]);

      if (x - prv[x]) {
        nxt += !cop[ans[x]][ans[prv[x]]];
        nxt += !cop[ans[x]][ans[::nxt[x]]];
      }
      if (y - prv[y]) {
        if (prv[y] - x) nxt += !cop[ans[y]][ans[prv[y]]];
        if (::nxt[y] - x) nxt += !cop[ans[y]][ans[::nxt[y]]];
      }

      int dt = val - nxt;
      if (Rand() > std::exp(dt / t))
        std::swap(ans[x], ans[y]);
      else
        val = nxt;
    }
    if (!val) break;
  }
  assert(!Eval());

  for (int i = 0, s = 0; i < m; s += l[i++])
    for (int j = s + 1; j <= s + l[i]; ++j)
      std::cout << ans[j] << "\n "[j < s + l[i]];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  Math();

  int t;
  std::cin >> t;
  while (t--) Proc();

  return 0;
}