#include <stdio.h>
#include <math.h>

const int P = 10;

int num[1000005];
int main () {
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; ++i) {
		scanf("%d", &num[i]);
	}

	int nP = N / P;

#define BEG(p) (p * nP)
#define END(p) ((p + 1) * nP)

	for (int p = 0; p < P; ++p) {
		int beg = BEG(p);
		int end = END(p);
		for (int n = beg; n < end - 1; ++n) {
			num[n + 1] += num[n];
		}
	}

	int step = 1;
	while (step < P) {
		for (int p = P - 1; p >= step; --p) {
			num[END(p) - 1] += num[END(p - step) - 1];
		}
		step *= 2;
	}

	for (int p = 1; p < P; ++p) {
		int beg = BEG(p);
		int end = END(p);
		for (int n = beg; n < end - 1; ++n) {
			num[n] += num[END(p - 1) - 1];
		}
	}

	for (int i = 0; i < N; ++i) {
		if (i > 0) {
			putchar(' ');
		}
		printf("%d", num[i]);
	}
	puts("");

	return 0;
}
