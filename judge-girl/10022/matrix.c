#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#define MAXMATRIX 2048

void multiply(int N, unsigned long A[][MAXMATRIX], unsigned long B[][MAXMATRIX], unsigned long C[][MAXMATRIX]) {
	// make B transpose
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < i; ++j) {
			unsigned long tmp = B[i][j];
			B[i][j] = B[j][i];
			B[j][i] = tmp;
		}
	}

#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			unsigned long sum = 0;	// overflow, let it go.
			for (int k = 0; k < N; k++)
				sum += A[i][k] * B[j][k];
			C[i][j] = sum;
		}
	}
}
