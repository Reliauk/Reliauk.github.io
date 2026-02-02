#include "non.h"
#include <cassert>
#include <cstdio>

int main() {
  int N, M;
  assert(2 == scanf("%d %d", &N, &M));

  std::vector<int> A(N);
  for (int i = 0; i < N; ++i) {
    assert(1 == scanf("%d", &A[i]));
  }

  std::vector<int> R = distribute_non(N, M, A);

  for (int k = 0; k < M; ++k) {
    printf("%d", R[k]);
    if (k + 1 < M) printf(" ");
  }
  printf("\n");

  return 0;
}

