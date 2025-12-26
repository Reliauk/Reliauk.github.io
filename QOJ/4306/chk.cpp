#include <bits/stdc++.h>

#include "testlib.h"

using namespace std;

const int N = 65;

int a[N][N], b[N][N];
set<string> s[N][N];

int main(int argc, char **argv) {
  ios::sync_with_stdio(false);
  registerInteraction(argc, argv);
  int n = inf.readInt();
  int Qlim = 100 * n * n;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) a[i][j] = inf.readInt();
  cout << n << endl;
  while (1) {
    string s;
    cin >> s;
    if (s == "?") {
      if (!Qlim) {
        cout << -1 << endl;
        quitf(_wa, "Query Limit Exceeded");
      }
      --Qlim;
      int la, lb;
      cin >> la >> lb;
      for (int i = 0; i < la; ++i) {
        string s;
        cin >> s;
        if (s.length() != lb) {
          cout << -1 << endl;
          quitf(_wa, "The given matrix from your query is invalid");
        }
        for (int j = 0; j < lb; ++j) {
          if (s[j] != '0' && s[j] != '1') {
            cout << -1 << endl;
            quitf(_wa, "The given matrix from your query is invalid");
          }
          b[i][j] = s[j] - '0';
        }
      }
      bool good = false;
      for (int sx = 0; sx + la - 1 < n; ++sx) {
        for (int sy = 0; sy + lb - 1 < n; ++sy) {
          bool ok = true;
          for (int i = 0; i < la && ok; ++i)
            for (int j = 0; j < lb && ok; ++j)
              if (a[sx + i][sy + j] != b[i][j]) ok = false;
          if (ok) {
            good = true;
            break;
          }
        }
        if (good) break;
      }
      if (good)
        cout << 1 << endl;
      else
        cout << 0 << endl;
    } else if (s == "!") {
      for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        if (s.length() != n) quitf(_wa, "Wrong Answer: not a n * n matrix");
        for (int j = 0; j < n; ++j) {
          if (s[j] != '0' && s[j] != '1')
            quitf(_wa, "Wrong Answer: not a 0/1 matrix");
          if (a[i][j] != s[j] - 48)
            quitf(_wa, "Wrong Answer: a[%d][%d] = %d, but your answer is %d\n",
                  i, j, a[i][j], s[j] - '0');
        }
      }
      quitf(_ok, "correct!");
    } else
      quitf(_wa, "invalid token %s", s.c_str());
  }
}
