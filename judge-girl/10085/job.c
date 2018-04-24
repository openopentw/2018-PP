#include "utils.h"
#define OFFSET 16

int ret[128];
int run(int n, int key) {
	int sum = 0;
	f(n, key, ret, ret + 1 * OFFSET, ret + 2 * OFFSET, ret + 3 * OFFSET);
	for (int i = 0; i < 4; i++)
		sum += ret[i * OFFSET];
	return sum;
}
