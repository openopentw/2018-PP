#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "matrix.h"

#define MAXMATRIX 2048
#define P 50

pthread_t threads[P];
pthread_attr_t attr;

typedef struct Args {
	int N;
	unsigned long (*A)[MAXMATRIX],
				  (*B)[MAXMATRIX],
				  (*C)[MAXMATRIX];
	int begRow, endRow;
} Args;

inline void set_up_cpu() {
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	for (int i = 0; i < P; i++)
		CPU_SET(i, &cpuset);
	assert(sched_setaffinity(0, sizeof(cpuset), &cpuset) == 0);
}

void *rowMulTrans (void *args) {
	int N = ((Args *)args)->N;
	unsigned long (*A)[MAXMATRIX] = ((Args *)args)->A,
				  (*B)[MAXMATRIX] = ((Args *)args)->B,
				  (*C)[MAXMATRIX] = ((Args *)args)->C;
	int begRow = ((Args *)args)->begRow,
		endRow = ((Args *)args)->endRow;

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

void multiply(int N, unsigned long A[][MAXMATRIX], unsigned long B[][MAXMATRIX], unsigned long C[][MAXMATRIX]) {
	// make B transpose
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < i; ++j) {
			unsigned long tmp = B[i][j];
			B[i][j] = B[j][i];
			B[j][i] = tmp;
		}
	}

	if (N < 100) { // avoid edge cases by single thread
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				unsigned long sum = 0;	// overflow, let it go.
				for (int k = 0; k < N; k++) {
					sum += A[i][k] * B[j][k];
				}
				C[i][j] = sum;
			}
		}
	} else {
		set_up_cpu();

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		int NP = N / P + (N % P != 0);
		Args argss[P];

		for (int i = 0; i < P; ++i) {
			argss[i].N = N;
			argss[i].A = A;
			argss[i].B = B;
			argss[i].C = C;
			argss[i].begRow = i * NP;
			int end = (i + 1) * NP;
			argss[i].endRow = (end < N)? end : N;

			pthread_create(&threads[i], &attr, rowMulTrans, (void *)&argss[i]);
		}

		for (int i = 0; i < P; i++) {
			pthread_join(threads[i], NULL);
		}

		pthread_attr_destroy(&attr);
	}
}
