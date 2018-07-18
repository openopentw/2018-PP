#include <CL/cl.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include "utils.h"

/* #define MOD2 1 */

#ifdef MYDEBUG
#define MyAssert(x) assert(x)
#else
#define MyAssert(x)
#endif

#define MAXGPU 10
#define MAXK 1024
#define MAXLOG 4096

#define MAXGLOBAL 8192
#define LOCALSIZE 1024
#define SEQSIZE 64
#define DEVICENUM 2

int main (void) {
	cl_int status;

	cl_platform_id platform_id;
	cl_uint platform_id_got;
	status = clGetPlatformIDs(1, &platform_id, &platform_id_got);
	MyAssert(status == CL_SUCCESS && platform_id_got == 1);

	cl_device_id GPU[MAXGPU];
	cl_uint GPU_id_got;
	status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &GPU_id_got);
	MyAssert(status == CL_SUCCESS);
	MyAssert(GPU_id_got >= DEVICENUM);

	cl_context context = clCreateContext(NULL, DEVICENUM, GPU, NULL, NULL, &status);
	MyAssert(status == CL_SUCCESS);

	cl_command_queue commandQueue[DEVICENUM];
	for (int device = 0; device < DEVICENUM; ++device) {
#ifdef MYDEBUG
		commandQueue[device] = clCreateCommandQueue(context, GPU[device], CL_QUEUE_PROFILING_ENABLE, &status);
#else
		commandQueue[device] = clCreateCommandQueue(context, GPU[device], 0, &status);
#endif
		MyAssert(status == CL_SUCCESS);
	}

	FILE *kernelfp = fopen("vecdot.cl", "r");
	MyAssert(kernelfp != NULL);
	char kernelBuffer[MAXK];
	const char *constKernelSource = kernelBuffer;
	size_t kernelLength = fread(kernelBuffer, 1, MAXK, kernelfp);

	cl_program program = clCreateProgramWithSource(context, 1, &constKernelSource, &kernelLength, &status);
	MyAssert(status == CL_SUCCESS);

	status = clBuildProgram(program, DEVICENUM, GPU, NULL, NULL, NULL);
#ifdef MYDEBUG
	if (status != CL_SUCCESS) {
		char log[MAXLOG];
		size_t logLength;
		for (int device = 0; device < DEVICENUM; device++) {
			clGetProgramBuildInfo(program, GPU[device], CL_PROGRAM_BUILD_LOG, MAXLOG, log, &logLength);
			puts(log);
		}
		exit(-1);
	}
#endif
	MyAssert(status == CL_SUCCESS);

	cl_kernel kernel = clCreateKernel(program, "vecDot", &status);
	MyAssert(status == CL_SUCCESS);

	uint32_t* ans[DEVICENUM];
	for (int device; device < DEVICENUM; ++device) {
		ans[device] = (uint32_t *)malloc(MAXGLOBAL * sizeof(uint32_t));
		MyAssert(ans[device] != NULL);
	}

	cl_mem bufferAns[DEVICENUM];
	for (int device; device < DEVICENUM; ++device) {
		bufferAns[device] = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
				MAXGLOBAL * sizeof(uint32_t), ans[device], &status);
		MyAssert(status == CL_SUCCESS);
	}

	uint32_t key1, key2;
	int N;
	while (scanf("%d %" PRIu32 " %" PRIu32, &N, &key1, &key2) == 3) {
		int NPerDevice = (N / DEVICENUM / SEQSIZE / LOCALSIZE + 1) * LOCALSIZE;
#ifdef MOD2
		int startId[] = {0, 1};
#else
		int startId[] = {0, NPerDevice};
#endif
		size_t globalThreads[] = {(size_t)NPerDevice};
		size_t localThreads[] = {LOCALSIZE};

		for (int device = 0; device < DEVICENUM; ++device) {
			status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufferAns[device]);
			MyAssert(status == CL_SUCCESS);
			status = clSetKernelArg(kernel, 1, sizeof(cl_uint), (void*)&key1);
			MyAssert(status == CL_SUCCESS);
			status = clSetKernelArg(kernel, 2, sizeof(cl_uint), (void*)&key2);
			MyAssert(status == CL_SUCCESS);
			status = clSetKernelArg(kernel, 3, sizeof(cl_uint), (void*)&startId[device]);
			MyAssert(status == CL_SUCCESS);
			status = clSetKernelArg(kernel, 4, sizeof(cl_uint), (void*)&N);
			MyAssert(status == CL_SUCCESS);

			status = clEnqueueNDRangeKernel(commandQueue[device], kernel, 1, NULL,
											globalThreads, localThreads, 0, NULL, NULL);
			MyAssert(status == CL_SUCCESS);
		}

		uint32_t sum = 0;
		for (int i = 2 * NPerDevice * SEQSIZE; i < N; ++i) {
			sum += encrypt(i, key1) * encrypt(i, key2);
		}

		for (int device = 0; device < DEVICENUM; ++device) {
			clEnqueueReadBuffer(commandQueue[device], bufferAns[device], CL_TRUE, 0,
								(NPerDevice / LOCALSIZE) * sizeof(cl_uint), ans[device], 0, NULL, NULL);
		}

		int endId = NPerDevice / LOCALSIZE;
		for (int device = 0; device < DEVICENUM; ++device) {
			for (int i = 0; i < endId; ++i) {
				sum += ans[device][i];
			}
		}

		printf("%" PRIu32 "\n", sum);
	}

	// TODO:
	// releases memory

	return 0;
}
