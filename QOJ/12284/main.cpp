#include <bits/stdc++.h>
#define ALL(v) begin(v), end(v)
#define PAR(v, l, r) data(v) + l, data(v) + r

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

const int N = 1E3 + 5;

int n, ans;
std::array<int, N + N> bg, ed, len;
std::array<std::vector<int>, N> in, out;

int To(int a, int b) { return (bg[b] - ed[a] + 24) % 24; }
std::array<int, N + N> anc;
int Anc(int o) { return o - anc[o] ? anc[o] = Anc(anc[o]) : o; }
void Merge(int u, int v) { anc[Anc(u)] = Anc(v); }
void Proc() {
  for (int i = 0; i < n + n; ++i) anc[i] = i;
  int cur = bg[out[1][0]] + To(in[1][1], out[1][1]);
  Merge(in[1][1], out[1][1]), Merge(in[1][0], out[1][0]);
  for (int i = 2; i <= n; ++i) {
    int v00 = To(in[i][0], out[i][0]);
    int v01 = To(in[i][0], out[i][1]);
    int v10 = To(in[i][1], out[i][0]);
    int v11 = To(in[i][1], out[i][1]);
    cur += std::min(v00 + v11, v01 + v10);
    if (v00 + v11 >= v01 + v10)
      Merge(in[i][0], out[i][1]), Merge(in[i][1], out[i][0]);
    if (v00 + v11 <= v01 + v10)
      Merge(in[i][0], out[i][0]), Merge(in[i][1], out[i][1]);
  }
  int cc = 0;
  for (int i = 0; i < n + n; ++i) cc += i == anc[i];
  ans = std::min(ans, cur + (cc - 1) * 24);
}

void Proc(int cs) {
  std::cin >> n, ans = 2E9;
  for (int i = 1; i <= n; ++i) in[i].clear(), out[i].clear();
  for (int i = 0, v; i < n + n; ++i) {
    std::cin >> v >> bg[i] >> len[i];
    ed[i] = (bg[i] + len[i]) % 24;
    out[i / 2 + 1].push_back(i);
    in[v].push_back(i);
  }
  Proc(), std::swap(out[1][0], out[1][1]), Proc();
  std::swap(in[1][0], in[1][1]), Proc();
  std::swap(out[1][0], out[1][1]), Proc();
  for (int i = 0; i < n + n; ++i) ans += len[i];
  std::cout << "Case #" << cs << ": " << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  for (int i = 1; i <= t; ++i) Proc(i);

  return 0;
}