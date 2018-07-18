#include <CL/cl.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include "utils.h"

#define MAXGPU 10
#define MAXK 1024
#define MAXGLOBAL 16390
#define LOCALSIZE 1024

int main (void) {
	cl_int status;
	cl_platform_id platform_id;
	cl_uint platform_id_got;
	status = clGetPlatformIDs(1, &platform_id, &platform_id_got);
	assert(status == CL_SUCCESS && platform_id_got == 1);

	cl_device_id GPU[MAXGPU];
	cl_uint GPU_id_got;
	status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &GPU_id_got);
	assert(status == CL_SUCCESS);

	GPU_id_got = 1;
	cl_context context = clCreateContext(NULL, 1, GPU, NULL, NULL, &status);
	assert(status == CL_SUCCESS);

	cl_command_queue commandQueue = clCreateCommandQueue(context, GPU[0], 0, &status);
	assert(status == CL_SUCCESS);

	// read kernel
	FILE *kernelfp = fopen("vecdot.cl", "r");
	assert(kernelfp != NULL);
	char kernelBuffer[MAXK];
	const char *constKernelSource = kernelBuffer;
	size_t kernelLength = fread(kernelBuffer, 1, MAXK, kernelfp);

	cl_program program = clCreateProgramWithSource(context, 1, &constKernelSource, &kernelLength, &status);
	assert(status == CL_SUCCESS);

	status = clBuildProgram(program, GPU_id_got, GPU, NULL, NULL, NULL);
	assert(status == CL_SUCCESS);

	cl_kernel kernel = clCreateKernel(program, "vecDot", &status);
	assert(status == CL_SUCCESS);

	cl_uint* ans = (cl_uint *)malloc(MAXGLOBAL * sizeof(cl_uint));
	cl_uint key1, key2;
	assert(ans != NULL);

		cl_mem bufferAns = clCreateBuffer(context,
				CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
				MAXGLOBAL * sizeof(cl_uint), ans, &status);
		assert(status == CL_SUCCESS);
	int N;
	while (scanf("%d%d%d", &N, &key1, &key2) == 3) {
		cl_mem bufferKey1 = clCreateBuffer(context,
				CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
				sizeof(cl_uint), &key1, &status);
		assert(status == CL_SUCCESS);
		cl_mem bufferKey2 = clCreateBuffer(context,
				CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
				sizeof(cl_uint), &key2, &status);
		assert(status == CL_SUCCESS);

		status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufferAns);
		assert(status == CL_SUCCESS);
		status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bufferKey1);
		assert(status == CL_SUCCESS);
		status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&bufferKey2);
		assert(status == CL_SUCCESS);

		size_t globalThreads[] = {(size_t)(N / LOCALSIZE) * LOCALSIZE};
		size_t localThreads[] = {LOCALSIZE};

		status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL,
									globalThreads, localThreads, 0, NULL, NULL);
		assert(status == CL_SUCCESS);

		clEnqueueReadBuffer(commandQueue, bufferAns, CL_TRUE, 0,
							(N / LOCALSIZE) * sizeof(cl_uint), ans, 0, NULL, NULL);

		uint32_t sum = 0;
		for (size_t i = 0; i < (N / LOCALSIZE); ++i) {
			sum += ans[i];
		}
		if (N % LOCALSIZE > 0) {
			for (int i = LOCALSIZE * (N / LOCALSIZE); i < N; ++i) {
				sum += encrypt(i, key1) * encrypt(i, key2);
			}
		}
		printf("%" PRIu32 "\n", sum);
	}

	// TODO:
	// releases memory

	return 0;
}
