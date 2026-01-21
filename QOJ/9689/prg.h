#ifndef PRG_H
#define PRG_H

#include <array>
#include <vector>

int solve(int n, int m, int k, std::vector<int> op,
          std::array<std::vector<int>, 3000> x, int c,
          std::array<std::array<int, 3000>, 25> s);

#endif  // PRG_H