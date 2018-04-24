#include <stdio.h>

#define MAXHW 512
#define INF 16384000005

#define square(x) ((x)*(x))

typedef long long ll;

int A[MAXHW][MAXHW], B[MAXHW][MAXHW];
int minJArr[MAXHW];
ll MinDiffArr[MAXHW];

int main (void) {
	int aH, aW, bH, bW;

	while (scanf("%d%d%d%d", &aH, &aW, &bH, &bW) == 4) {
		// input
		for (int i = 0; i < aH; ++i) {
			for (int j = 0; j < aW; ++j) {
				scanf("%d", &A[i][j]);
			}
		}
		for (int i = 0; i < bH; ++i) {
			for (int j = 0; j < bW; ++j) {
				scanf("%d", &B[i][j]);
			}
		}
		// calculate
#pragma omp parallel for
		for (int i = 0; i <= aH - bH; ++i) {
			ll minDiff = INF;
			int minJ;
			for (int j = 0; j <= aW - bW; ++j) {
				ll diff = 0;
				for (int h = 0; h < bH; ++h) {
					for (int w = 0; w < bW; ++w) {
						diff += square(A[i + h][j + w] - B[h][w]);
					}
				}
#ifdef DEBUG
				printf("i, j, diff = %d, %d, %lld\n", i, j, diff);
#endif
				if (diff < minDiff) {
					minDiff = diff;
					minJ = j;
				}
			}
			minJArr[i] = minJ;
			MinDiffArr[i] = minDiff;
		}
		// compare
		int minI;
		ll minDiff = INF;
		for (int i = 0; i <= aH - bH; ++i) {
			if (MinDiffArr[i] < minDiff) {
				minI = i;
				minDiff = MinDiffArr[i];
			}
		}
		int minJ = minJArr[minI];
		// output
		printf("%d %d\n", minI + 1, minJ + 1);
	}

	return 0;
}
