#include "paper_mills.h"
#include <cassert>
#include <cstdio>

int main() {
  int N, M;
  assert(2 == scanf("%d %d", &N, &M));

  std::vector<int> A(N);
  for (int i = 0; i < N; ++i) {
    assert(1 == scanf("%d", &A[i]));
  }

  std::vector<int> D(M);
  for (int j = 0; j < M; ++j) {
    assert(1 == scanf("%d", &D[j]));
  }

  std::vector<long long> ans = solve_paper_mills(N, A, M, D);

  for (int j = 0; j < M; ++j) {
    printf("%lld\n", ans[j]);
  }

  return 0;
}

