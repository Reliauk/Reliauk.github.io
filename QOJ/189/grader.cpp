#include "shop.h"

#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <cmath>
#include <ctime>
using namespace std;

namespace Interactive_Library {

	#define assertf(k, A...) ({ (k) ? void() : quitf(0, A); })
	#define assert(k) assertf(k, "Assertion \"%s\" failed.", #k)
	#define quitf(x, A...) ({ printf("%.4lf ", (double) (x)); printf(A); puts(""); exit(0); })

	const int MAX_N = 100000;
	const int MAX_M = 1000000;

	int N, M, type, A[MAX_N], Cnt;
	int vis[MAX_N], timer;

	inline int Rand(int l, int r) { return rand() % (r - l + 1) + l; }

	int query(int *S, int nS, int *T, int nT) {
		Cnt += nS + nT;
		assertf(Cnt <= M, "Query limit exceeded.");

		int cnt1 = 0, cnt2 = 0;
		bool flag = 0;

		timer++;

		for (int i = 0; i < nS; ++i) {
			int x = S[i];
			if (x < 0 or x > N - 1 or vis[x] == timer) {
				flag = 1;
				goto END;
			}
			vis[x] = timer;
			cnt1 += A[x];
		}

		timer++;

		for (int i = 0; i < nT; ++i) {
			int x = T[i];
			if (x < 0 or x > N - 1 or vis[x] == timer) {
				flag = 1;
				goto END;
			}
			vis[x] = timer;
			cnt2 += A[x];
		}

		END: 
		assertf(flag == 0, "Invalid call to query");

		if (cnt1 != cnt2) return cnt1 < cnt2;
		return Rand(0, 1);
	}

	void __main__ () {
		int T, type;

		assert(cin >> T);
		assert(1 <= T and T <= 10);
		assert(cin >> type);
		assert(1 <= type and type <= 5);

		for (int i = 0; i < T; ++i) {

			Cnt = 0;

			assert(cin >> N);
			assert(1 <= N and N <= MAX_N);
			assert(cin >> M);
			assert(1 <= M and M <= MAX_M);

			// N = 100000;
			// M = 500100;

			string str;
			assert(cin >> str);
			assert(str.size() == N);

			int C = 0;
			for (int i = 0; i < N; ++i) {
				A[i] = str[i] - '0';
				assert(0 <= A[i] and A[i] <= 1);
				C ^= A[i];
			}

			static int ans[MAX_N];

			for (int i = 0; i < N; ++i) ans[i] = 0;
			find_price(type, N, C, ans);

			for (int i = 0; i < N; ++i) {
				assertf(A[i] == ans[i], "Your answer is incorrect.");
			}

			printf("Testcase #%d correct, total cost of queries: %d.\n", i + 1, Cnt);

		}

		quitf(1.0, "Correct Answer");
	}

}

int query(int *S, int nS, int *T, int nT) {
	return Interactive_Library::query(S, nS, T, nT);
}

int main() {
	//freopen("shop.in","r",stdin);
	//freopen("shop.out","w",stdout);
	int T = time(0);
	srand(T);
	Interactive_Library::__main__();
}
