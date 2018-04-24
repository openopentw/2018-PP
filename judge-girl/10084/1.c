#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <assert.h>

#include "utils.h"

#define MAXN 10000005
#define P 6

uint32_t key;
uint32_t arr[MAXN];
int n, nP;
int args[P];
pthread_t threads[P];
pthread_attr_t attr;

void *calcPartialPrefixSum(void *threadId) {
	int id = *(int *)threadId;
	int beg = id * nP + 1;
	int end = (id + 1) * nP + 1;
	end = (end < n)? end : n;

	uint32_t sum = 0;
	for (int i = beg; i < end; i++) {
		sum += encrypt(i, key);
		arr[i] = sum;
	}

	pthread_exit(NULL);
}

void *partialAddPrevLast(void *threadId) {
	int id = *(int *)threadId;
	int beg = id * nP + 1;
	int end = (id + 1) * nP;
	end = (end < n)? end : n;
	uint32_t last = arr[beg - 1];

	for (int i = beg; i < end; i++) {
		arr[i] += last;
	}

	pthread_exit(NULL);
}

inline void parallelSolve() {
	++n;

	nP = n / P + (n % P != 0);

	for (int i = 0; i < P; ++i) {
		args[i] = i;
		pthread_create(&threads[i], &attr, calcPartialPrefixSum, (void *)&args[i]);
	}

	for (int i = 0; i < P; i++) {
		pthread_join(threads[i], NULL);
	}

	/* calculate the prefix sum of last numbers */
	for (int i = 1; i < P; i++) {
		int thisLast = (i + 1) * nP;
		thisLast = (thisLast < n)? thisLast : n;
		int prevLast = i * nP;
		arr[thisLast] += arr[prevLast];
	}

	/* add each one with last of prev-thread */
	for (int i = 0; i < P; ++i) {
		pthread_create(&threads[i], &attr, partialAddPrevLast, (void *)&args[i]);
	}

	for (int i = 0; i < P; i++) {
		pthread_join(threads[i], NULL);
	}

	output(arr, n - 1);
}

int main() {
	{
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		for (int i = 0; i < P; i++)
			CPU_SET(i, &cpuset);
		assert(sched_setaffinity(0, sizeof(cpuset), &cpuset) == 0);
	}

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	while (scanf("%d %" PRIu32, &n, &key) == 2) {
		if (n < 100) { // avoid edge cases by single thread
			uint32_t sum = 0;
			for (int i = 1; i <= n; i++) {
				sum += encrypt(i, key);
				arr[i] = sum;
			}
			output(arr, n);
		} else {
			parallelSolve();
		}
	}

	pthread_attr_destroy(&attr);

	return 0;
}
