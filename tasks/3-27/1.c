#include <stdio.h>
#include <math.h>

const int P = 10;

int num[1000005];
int last[10];
int last2[10];
int main () {
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; ++i) {
		scanf("%d", &num[i]);
	}

	int nP = N / P;

	for (int p = 0; p < P; ++p) {
		int beg = p * nP;
		int end = (p + 1) * nP;
		for (int n = beg; n < end - 1; ++n) {
			num[n + 1] += num[n];
		}
		last[p] = num[end - 1];
		last2[p] = num[end - 1];
	}

	for (int expo = 0; pow(2, expo) < P; ++expo) {
		int step = pow(2, expo);
		for (int p = step; p < P; ++p) {
			last2[p] += last[p - step];
		}
		for (int p = step; p < P; ++p) {
			last[p] = last2[p];
		}
	}

	for (int p = 0; p < P; ++p) {
		int end = (p + 1) * nP;
		num[end - 1] = last[p];
	}

	for (int p = 1; p < P; ++p) {
		int beg = p * nP;
		int end = (p + 1) * nP;
		for (int n = beg; n < end - 1; ++n) {
			num[n] += last[p - 1];
		}
	}

	for (int i = 0; i < N; ++i) {
		printf("%d", num[i]);
		if (i < N - 1) {
			putchar(' ');
		}
	}
	puts("");

	return 0;
}
