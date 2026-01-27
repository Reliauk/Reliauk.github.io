#include <bits/stdc++.h>

int main() {
  puts("Good Luck & Have Fun!");
  return 0;
}

int pigeon_num(int, int);
std::string send(int, int, int, __uint128_t);
__uint128_t receive(int, int, const std::string &);

class Grader {
  int Taskid, k, T;
  __uint128_t msg[1005];
  int readint() {
    int n = 0, F = 0;
    char ch;
    while ((ch = getchar()) != '-' && (ch < '0' || ch > '9'));
    ch == '-' ? F = 1 : n = ch - '0';
    while ((ch = getchar()) >= '0' && ch <= '9') n = n * 10 + ch - '0';
    return F ? -n : n;
  }
  __uint128_t readuint128() {
    __uint128_t n = 0;
    char ch;
    while ((ch = getchar()) < '0' || ch > '9');
    n = ch - '0';
    while ((ch = getchar()) >= '0' && ch <= '9') n = n * 10 + ch - '0';
    return n;
  }
  void printuint128(__uint128_t x) {
    const unsigned long long Base = 10000000000000llu;
    unsigned long long a[3];
    a[0] = x / Base / Base;
    a[1] = x / Base % Base;
    a[2] = x % Base;
    int i = 0;
    while (i != 2 && a[i] == 0) {
      ++i;
    }
    for (printf("%lld", a[i++]); i <= 2; ++i) {
      printf("%013lld", a[i]);
    }
    puts("");
  }
  void error(const std::string &info) {
    std::cout << info << std::endl;
    exit(0);
  }
  std::string fly(const std::string &order) {
    static std::mt19937_64 gen(351493);
    auto randint = [&](int l, int r) { return gen() % (r - l + 1) + l; };
    std::string ret;
    int n = order.length();
    std::vector<int> p, vis(n, 0);
    for (int i = 0; i < n; ++i) {
      int now;
      if (i >= k && !vis[i - k]) {
        now = i - k;
      } else {
        int lb = std::max(0, i - k);
        int ub = std::min(n - 1, i + k);
        do {
          now = randint(lb, ub);
        } while (vis[now]);
      }
      vis[now] = 1;
      p.push_back(now);
    }
    for (int i = 0; i < n; ++i) {
      ret += order[p[i]];
    }
    return ret;
  }

 public:
  Grader() {
    static std::mt19937_64 gen(351493);
    Taskid = readint();
    k = readint();
    T = readint();
    for (int i = 1; i <= T; ++i) {
      msg[i] = readuint128();
    }
    fclose(stdin);
    int n = pigeon_num(Taskid, k);
    if (n < 1 || n > 4000) {
      error("Invalid number of pigeons.");
    }
    for (int i = 1; i <= T; ++i) {
      std::string order = send(Taskid, n, k, msg[i]);
      for (auto c : order) {
        if (c != '0' && c != '1') {
          error("Invalid color of pigeon.");
        }
      }
      printf("Message:    ");
      printuint128(msg[i]);
      std::cout << "Taking off: " << order << std::endl;
      if (order.length() != n) {
        error("Too few or too many pigeons taking off.");
      }
      order = fly(order);
      std::cout << "Landing:    " << order << std::endl;
      __uint128_t received = receive(Taskid, k, order);
      printf("Received:   ");
      printuint128(received);
      puts("");
      if (received != msg[i]) {
        error("Recieved wrong message.");
      }
    }
    if (n == 1) {
      printf("Accepted using 1 pigeon.\n");
    } else {
      printf("Accepted using %d pigeons.\n", n);
    }
    exit(0);
  }
} grader;