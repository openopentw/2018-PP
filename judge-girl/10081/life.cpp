#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAXN 2048
#define SIDE (MAXN + 2)

#define nLiveNeighbor(A, i, j) \
	A[i + 1][j] + A[i - 1][j] + A[i][j + 1] + \
	A[i][j - 1] + A[i + 1][j + 1] + A[i + 1][j - 1] + \
	A[i - 1][j + 1] + A[i - 1][j - 1]

char A[SIDE][SIDE];
char B[SIDE][SIDE];

void print (char A[SIDE][SIDE], int n) {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			printf("%d", A[i][j]);
		}
		puts("");
	}
}

int main() {
	int n, generation;

	/* read input */
	scanf("%d%d", &n, &generation);
	for (int i = 1; i <= n; i++) {
		char inputStr[MAXN];
		scanf("%s", inputStr);
		for (int j = 1; j <= n; j++) {
			A[i][j] = inputStr[j - 1] - '0';
		}
	}

	/* generation */
#pragma omp parallel
	for (int g = 0; g < generation; g++) {
		if (g % 2 == 0) {
#pragma omp for // from A to B
			for (int i = 1; i <= n; i++) {
				for (int j = 1; j <= n; j++) {
					int nln = nLiveNeighbor(A, i, j);
					B[i][j] = ((A[i][j] == 0 && nln == 3) ||
						(A[i][j] == 1 && (nln == 2 || nln == 3)));
				}
			}
		} else {
#pragma omp for // from B to A
			for (int i = 1; i <= n; i++) {
				for (int j = 1; j <= n; j++) {
					int nln = nLiveNeighbor(B, i, j);
					A[i][j] = ((B[i][j] == 0 && nln == 3) ||
						(B[i][j] == 1 && (nln == 2 || nln == 3)));
				}
			}
		}
	}

	/* printcell */
	if (generation % 2 == 0) {
		print(A, n);
	} else {
		print(B, n);
	}

	return 0;
}
