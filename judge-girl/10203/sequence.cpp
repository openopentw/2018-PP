#include <stdio.h>
#include <omp.h>

#define DPARR(i, j) dp[(i) * N + (j)]

#define MAXN 2048
#define INF (1LL<<60)

int N;
long long dp[MAXN * MAXN], SZ[MAXN + 1];

int main () {
	while (scanf("%d", &N) == 1) {
		// input
		for (int i = 0; i <= N; i++) {
			scanf("%lld", &SZ[i]);
		}

		// initialize dp
		for (int i = 0; i < N; i++) {
			DPARR(i, i) = 0;
		}

		// dp
#pragma omp parallel
		for (int len = 1; len < N; len++) {
#pragma omp for
			for (int l = 0; l < N - len; l++) {
				int r = l + len;
				long long local = INF;
				for (int k = l; k < r; k++) {
					long long t = DPARR(l, k) + DPARR(r, k + 1) + SZ[l] * SZ[k + 1] * SZ[r + 1];
					if (t < local) {
						local = t;
					}
				}
				DPARR(l, r) = local;
				DPARR(r, l) = local;
			}
		}
		printf("%lld\n", DPARR(0, N - 1));
	}

	return 0;
}
