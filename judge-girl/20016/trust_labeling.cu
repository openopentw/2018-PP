#include <stdio.h>
#include <thrust/execution_policy.h>

using namespace thrust;

template<class T> struct MM {
    const char *base;
    MM(const char *base): base(base) {}
    __host__ __device__ T operator()(const T& index) {
        return (base[index] == ' ')? index : -1;
    };
};

template<class T> struct MM1 {
    const int *base;
    MM1(int *base): base(base) {}
    __host__ __device__ T operator()(const T& index) {
        return index - base[index];
    };
};

void labeling (const char *cuStr, int *cuPos, int strLen) {
	tabulate(device, cuPos, cuPos + strLen, MM<int32_t>(cuStr));
	inclusive_scan(device, cuPos, cuPos + strLen, cuPos, maximum<int>());
	tabulate(device, cuPos, cuPos + strLen, MM1<int32_t>(cuPos));
}
