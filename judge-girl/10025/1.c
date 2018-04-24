#include <stdio.h>

#define MAXHW 505
#define INF 2147483647

#define square(x) ((x)*(x))

int A[MAXHW][MAXHW], B[MAXHW][MAXHW];

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
		int minDiff = INF;
		int minI, minJ;
		for (int i = 0; i <= aH - bH; ++i) {
			for (int j = 0; j <= aW - bW; ++j) {
				int diff = 0;
				for (int h = 0; h < bH; ++h) {
					for (int w = 0; w < bW; ++w) {
						diff += square(A[i + h][j + w] - B[h][w]);
					}
				}
#ifdef DEBUG
				printf("i, j, diff = %d, %d, %d\n", i, j, diff);
#endif
				if (diff < minDiff) {
					minDiff = diff;
					minI = i;
					minJ = j;
				}
			}
		}
		// output
		printf("%d %d\n", minI + 1, minJ + 1);
	}

	return 0;
}
