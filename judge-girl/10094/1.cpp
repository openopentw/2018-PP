#include <stdio.h>

#define MAXN 10005
#define MAXW 1000005

int N, M;
int W[MAXN], V[MAXN];

int DP[2][MAXW];

inline int max (int a, int b) {
	return (a > b)? a : b;
}

int main () {
	scanf("%d%d", &N, &M);
	for (int i = 0; i < N; ++i) {
		scanf("%d%d", &W[i], &V[i]);
	}

	int nowN = 0;
	for (int n = 0; n < N; ++n) {
#pragma omp parallel for
		for (int w = 1; w < M + 1; ++w) {
			if (w < W[n]) {
				DP[nowN][w] = DP[nowN ^ 1][w];
			} else {
				DP[nowN][w] = max(
						DP[nowN ^ 1][w],
						DP[nowN ^ 1][w - W[n]] + V[n]
					);
			}
		}

		nowN ^= 1;
	}

	printf("%d\n", DP[nowN ^ 1][M]);
	return 0;
}
