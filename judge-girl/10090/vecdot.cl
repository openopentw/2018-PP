#define uint32_t unsigned int
#define LOCALSIZE 1024

static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}

static inline uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}

__kernel void vecDot (__global uint32_t* ans,
		      __global uint32_t* key1,
		      __global uint32_t* key2) {
    int globalId = get_global_id(0);
    int groupId = get_group_id(0);
    int localId = get_local_id(0);
    int localSize = get_local_size(0);

    __local uint32_t buf[LOCALSIZE];

    buf[localId] = encrypt(globalId, *key1) * encrypt(globalId, *key2);

    barrier(CLK_LOCAL_MEM_FENCE);
    for (int i = LOCALSIZE >> 1; i; i >>= 1) {
	if (localId < i) {
	    buf[localId] += buf[localId + i];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (localId == 0) {
	ans[groupId] = buf[0];
    }
}
