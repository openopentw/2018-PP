#include <stdio.h>

#define MAXTHREAD 2000

#define MAXN 16

bool obsta[MAXN][MAXN];
int N;
int putQueen[MAXN];

/* ok */
int ok(int position[], int next, int test)
{
  for (int i = 0; i < next; i++)
    if (position[i] == test || 
	(abs(test - position[i]) == next - i))
      return 0;
  return 1;
}

/* queen */
int queen(int position[], int next)
{
  if (next >= n)
    return 1;
  int sum = 0;
  for (int test = 0; test < n; test++) 
    if (ok(position, next, test)) {
      position[next] = test;
      sum += queen(position, next + 1);
    }
  return sum;
}

int main () {
	while(scanf("%d", &N) != EOF) {
		char tmp[MAXN + 1];
		for (int i = 0; i < N; ++i) {
			scanf("%s", tmp);
			for (int j = 0; j < N; ++j) {
				if (tmp[j] == '.') {
					obsta[i][j] = true;
				} else {
					obsta[i][j] = false;
				}
			}
		}

		for (int i = 0; i < 3; ++i) {
		}

	}
	return 0;
}
