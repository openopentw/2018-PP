#include <stdio.h>
#include <stdlib.h>

#define MAXN 16

bool obsta[MAXN][MAXN];

int position[MAXN];
int n;

int ok(int position[], int next, int test) {
	for (int i = 0; i < next; i++) {
		if (position[i] == test ||
			(abs(test - position[i]) == next - i)) {
			return 0;
		}
	}
	return 1;
}

int queen(int position[], int next) {
	if (next >= n) {
		return 1;
	}
	int sum = 0;
	for (int test = 0; test < n; test++) {
		if (ok(position, next, test) &&
			!obsta[test][next]) {
			position[next] = test;
			sum += queen(position, next + 1);
		}
	}
	return sum;
}

int main () {
	for (int t = 1; scanf("%d", &n) != EOF; t++) {
		char tmp[MAXN + 1];
		for (int i = 0; i < n; ++i) {
			scanf("%s", tmp);
			for (int j = 0; j < n; ++j) {
				obsta[i][j] = (tmp[j] == '*');
			}
		}

		/* loop */
		int numSolution = 0;
#pragma omp parallel for collapse(3) private (position) reduction(+ : numSolution) schedule(dynamic, 1)
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < n; k++) {
					if (j == i || k == i || k == j ||
						abs(j - i) == 1 ||
						abs(k - j) == 1 ||
						abs(k - i) == 2 ||
						obsta[i][0] ||
						obsta[j][1] ||
						obsta[k][2]) {
						continue;
					}
					position[0] = i;
					position[1] = j;
					position[2] = k;
					int num = queen(position, 3);
					numSolution += num;
				}
			}
		}

		printf("Case %d: %d\n", t, numSolution);
	}
	return 0;
}
