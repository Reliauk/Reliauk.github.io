#include <bits/stdc++.h>

#include "mpc.h"


const int N = 22000, M = 2200;

namespace grader {
int score[10] = {100, 13, 11, 8, 5, 5, 4, 4, 3, 3};
std::mt19937_64 rnd(time(0));
struct bigint {
  int val[M];
  void clear() { memset(val, 0, sizeof(val)); }
  void gen(int len) {
    for (int i = 0; i < len; i++) val[i] = rnd() % 2;
  }
  void read(int len) {
    for (int i = 0; i < len; i++) std::cin >> val[i];
  }
  void print(int len) {
    for (int i = 0; i < len; i++) printf("%d", val[i]);
    printf("\n");
  }
} num[N], correct_result;
bigint plus(bigint x, bigint y) {
  int carry = 0;
  bigint ans;
  ans.clear();
  for (int i = 0; i < M; i++) {
    ans.val[i] = (x.val[i] + y.val[i] + carry) % 2;
    carry = (x.val[i] + y.val[i] + carry) / 2;
  }
  return ans;
}
int n, m, ans;
::player player[N];
void gen_data(int n, int m) {
  for (int i = 0; i < n; i++) num[i].read(m);
  correct_result.clear();
  for (int i = 0; i < n; i++) correct_result = plus(correct_result, num[i]);
}
void grade() {
  std::cin >> n >> m;
  ans = precalc(n, m);
  gen_data(n, m);
  for (int i = 0; i < n; i++) {
    player[i].memory.fill(0);
    for (int j = 0; j < M; j++) player[i].memory[j] = num[i].val[j];
  }
  for (int i = 1; i <= ans; i++) {
    for (int j = n; j >= 0; j--) {
      bool curr = transmit(player[j], i, j);
      if (j < n) player[j + 1].last_message = curr;
    }
  }
  bool correct = 1;
  correct_result.print(M);
  for (int i = 0; i < M; i++)
    correct &= (player[n].memory[i] == correct_result.val[i]);
  for (int i = 0; i < M; i++) printf("%d", player[n].memory[i]);
  puts("");
  if (!correct) {
    printf("Your answer is not correct.");
  } else {
    int score_ = 0;
    for (int i = 0; i <= 9; i++)
      if (n + m + score[i] >= ans) score_++;
    printf("OK, you get %d%% points", score_ * 10);
  }
}
}  // namespace grader

int main() {
  std::ios::sync_with_stdio(0);
  std::cin.tie(0);
  grader::grade();
  return 0;
}
