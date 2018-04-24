#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#define MAXMATRIX 2048
#define P 50

void rowMultiTrans (int N,
					unsigned long A[][MAXMATRIX],
					unsigned long B[][MAXMATRIX],
					unsigned long C[][MAXMATRIX],
					int begRow,
					int endRow) {
	for (int i = begRow; i < endRow; ++i) {
		for (int j = 0; j < N; ++j) {
			unsigned long sum = 0; // overflow, let it go.
			for (int k = 0; k < N; k++) {
				sum += A[i][k] * B[j][k];
			}
			C[i][j] = sum;
		}
	}
}

void multiply (int N,
			   unsigned long A[][MAXMATRIX],
			   unsigned long B[][MAXMATRIX],
			   unsigned long C[][MAXMATRIX]) {
	// make B transpose
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < i; ++j) {
			unsigned long tmp = B[i][j];
			B[i][j] = B[j][i];
			B[j][i] = tmp;
		}
	}

	int NP = N / P + (N % P != 0);

	for (int i = 0; i < P; ++i) {
		int beg = i * NP;
		int end = (i + 1) * NP;
		end = (end < N)? end : N;

		rowMultiTrans(N, A, B, C, beg, end);
	}
}
