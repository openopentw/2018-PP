#include <stdio.h>
#include <assert.h>
#include <CL/cl.h>

#define MAXGPU 10
#define MAXK 1024

int main (void) {
  char kernelFileName[32];
  scanf("%s", kernelFileName);

  cl_int status;
  cl_platform_id platform_id;
  cl_uint platform_id_got;
  status = clGetPlatformIDs(1, &platform_id, &platform_id_got);
  assert(status == CL_SUCCESS && platform_id_got == 1);

  cl_device_id GPU[MAXGPU];
  cl_uint GPU_id_got;
  status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &GPU_id_got);
  assert(status == CL_SUCCESS);

  /* getcontext */
  cl_context context = clCreateContext(NULL, GPU_id_got, GPU, NULL, NULL, &status);
  assert(status == CL_SUCCESS);

  /* commandqueue */
  cl_command_queue commandQueue = clCreateCommandQueue(context, GPU[0], 0, &status);
  assert(status == CL_SUCCESS);

  /* kernelsource */
  FILE *kernelfp = fopen(kernelFileName, "r");
  assert(kernelfp != NULL);
  char kernelBuffer[MAXK];
  const char *constKernelSource = kernelBuffer;
  size_t kernelLength = fread(kernelBuffer, 1, MAXK, kernelfp);

  cl_program program = clCreateProgramWithSource(context, 1, &constKernelSource, &kernelLength, &status);
  assert(status == CL_SUCCESS);

  /* buildprogram */
  status = clBuildProgram(program, GPU_id_got, GPU, NULL, NULL, NULL);
  /* assert(status == CL_SUCCESS); */

  /* getprogrambuildinfo */
  char ret_param_value[4096];
  size_t ret_param_value_size;
  status = clGetProgramBuildInfo(program, GPU[0], CL_PROGRAM_BUILD_LOG,
      4096, ret_param_value, &ret_param_value_size);
  assert(status == CL_SUCCESS);

  ret_param_value[ret_param_value_size] = '\0';
  printf("%s", ret_param_value);

  return 0;
}
