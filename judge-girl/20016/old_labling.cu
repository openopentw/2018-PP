#include <stdio.h>

#define BLOCKSIZE 512

__global__ void partialLabeling (const char *cuStr, int *cuPos, int strLen) {
	int begId = blockIdx.x * BLOCKSIZE;
	int endId = (blockIdx.x + 1) * BLOCKSIZE;
	endId = (endId < strLen)? endId : strLen;

	int sum = 0;
	for (int i = begId; i < endId; ++i) {
		if (cuStr[i] == ' ') {
			sum = 0;
		} else {
			++sum;
		}
		cuPos[i] = sum;
	}
}

__global__ void prefixLabeling (const char *cuStr, int *cuPos, int strLen) {
	int begId = (blockIdx.x + 1) * BLOCKSIZE;
	int endId = (blockIdx.x + 2) * BLOCKSIZE;
	endId = (endId < strLen)? endId : strLen;

	if (cuStr[begId - 1] != ' ') {
		int prevPos = cuPos[begId - 1];
		for (int i = begId; i < endId && cuStr[i] != ' '; ++i) {
			cuPos[i] += prevPos;
		}
	}
}

void labeling (const char *cuStr, int *cuPos, int strLen) {
	int block_dim = strLen / BLOCKSIZE;

	partialLabeling <<<block_dim + 1, 1>>> (cuStr, cuPos, strLen);
	cudaThreadSynchronize();

	prefixLabeling <<<block_dim, 1>>> (cuStr, cuPos, strLen);
	cudaThreadSynchronize();
}
