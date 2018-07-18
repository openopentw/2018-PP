#include <stdio.h>
#include <stdlib.h>

#define MAXN 20

char obsta[MAXN][MAXN];

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
		if (obsta[test][next] == '.' &&
			ok(position, next, test)) {
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
			scanf("%s", obsta[i]);
		}

		/* loop */
		int numSolution = 0;
#pragma omp parallel for collapse(3) schedule(dynamic) private (position) reduction(+ : numSolution)
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < n; k++) {
					if (obsta[i][0] == '.') {
						position[0] = i;
						if (obsta[j][1] == '.' && ok(position, 1, j)) {
							position[1] = j;
							if (obsta[k][2] == '.' && ok(position, 2, k)) {
								position[2] = k;
								numSolution += queen(position, 3);
							}
						}
					}
				}
			}
		}

		printf("Case %d: %d\n", t, numSolution);
	}
	return 0;
}
