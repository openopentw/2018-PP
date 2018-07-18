#include <stdio.h>

#define BLOCKDIM 512

__global__ void partial (const char *cuStr, int *cuPos, int strLen) {
	int tid = threadIdx.x;
	int gid = blockIdx.x * blockDim.x + threadIdx.x;
	__shared__ int buf[BLOCKDIM];
	if (gid > strLen) {
		return ;
	}

	buf[tid] = (cuStr[gid] == ' ') ? tid : -1;
	for (int i = 1; i <= tid; i <<= 1) {
		__syncthreads();
		int tmp = buf[tid - i];
		__syncthreads();
		buf[tid] = (buf[tid] > tmp)? buf[tid] : tmp;
	}
	__syncthreads();
	cuPos[gid] = tid - buf[tid];
}

__global__ void fix (int *cuPos, int strLen) {
	int tid = threadIdx.x;
	int gid = blockIdx.x * blockDim.x + threadIdx.x;

	if (gid < strLen && cuPos[gid] == tid + 1) {
		cuPos[gid] += cuPos[gid - tid - 1];
	}
}

void labeling (const char *cuStr, int *cuPos, int strLen) {
	int block_dim = strLen / BLOCKDIM;
	partial <<<block_dim + 1, BLOCKDIM>>> (cuStr, cuPos, strLen);
	fix <<<block_dim, BLOCKDIM>>> (cuPos + BLOCKDIM, strLen - BLOCKDIM);
}
