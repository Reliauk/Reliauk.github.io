#include <bits/stdc++.h>

#include "prg.h"

int main() {
  unsigned long long seed = 0;
  std::cin >> seed;
  std::mt19937_64 rnd(seed);

  int k, type;
  std::cin >> k >> type;
  assert(k == 2 or k == 3 or k == 4);
  assert(type == 0 or type == 1);

  std::vector<int> op(k - 1), op2(k - 1, 2);
  if (type == 1) {
    for (int i = 0; i <= k - 2; i++) {
      std::cin >> op[i];
      assert(op[i] == 0 or op[i] == 1 or op[i] == 2);
    }
    assert(op != op2);
  }

  int w = 0;
  for (int T = 0; T < 100; T++) {
    if (type == 0) {
      do {
        for (int i = 0; i <= k - 2; i++) op[i] = rnd() % 3;
      } while (op == op2);
    }

    int n = 1000, m = 3000, c = 25;
    std::array<std::vector<int>, 3000> x;
    std::array<std::array<int, 3000>, 25> s;

    for (int i = 0; i < m; i++) {
      x[i] = std::vector<int>(k);
      for (int j = 0; j < k; j++) {
        do {
          x[i][j] = rnd() % n;
        } while ([&]() {
          for (int l = 0; l < j; l++)
            if (x[i][l] == x[i][j]) return true;
          return false;
        }());
      }
    }

    int r = rnd() % 2 + 1;
    if (r == 1) {
      for (int i = 0; i < c; i++)
        for (int j = 0; j < m; j++) s[i][j] = rnd() % 2;
    } else {
      for (int i = 0; i < c; i++) {
        std::array<int, 1000> t;
        for (int j = 0; j < n; j++) t[j] = rnd() % 2;
        for (int j = 0; j < m; j++) {
          s[i][j] = t[x[j][0]];
          for (int l = 0; l <= k - 2; l++) {
            if (op[l] == 0)
              s[i][j] &= t[x[j][l + 1]];
            else if (op[l] == 1)
              s[i][j] |= t[x[j][l + 1]];
            else
              s[i][j] ^= t[x[j][l + 1]];
          }
        }
      }
    }

    int tmp = r != solve(n, m, k, op, x, c, s);
    w += tmp;
    if (tmp)
      std::cerr << op[0] << ' ' << op[1] << ' ' << op[2] << "HH\n"
                << r << "GG\n";
  }

  double score = 0;
  if (k == 2)
    score = 5;
  else if (k == 3)
    score = 15;
  else if (type == 0)
    score = 30;
  else
    score = 50;
  score *= std::max(0.0, 1 - log2(std::max(w, 1)) / 4);
  std::cout << "You are correct in " << 100 - w << " out of 100 tests,"
            << " scoring " << std::fixed << std::setprecision(2) << score
            << " points." << std::endl;
  return 0;
}
