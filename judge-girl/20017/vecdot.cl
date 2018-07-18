#define uint32_t unsigned int
#define LOCALSIZE 1024
#define SEQSIZE 64
/* #define SEQSIZE 1 */

static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
	return  (x << n) | (x >> (32-n));
}

static inline uint32_t encrypt(uint32_t m, uint32_t key) {
	return (rotate_left(m, key&31) + key)^key;
}

__kernel void vecDot (__global uint32_t* ans,
					  __global uint32_t* key1,
					  __global uint32_t* key2) {
	__local uint32_t buf[LOCALSIZE];

	int globalId = get_global_id(0);
	int blockId = globalId * SEQSIZE;

	uint32_t sum = 0;
	for (int i = 0; i < SEQSIZE; ++i) {
		sum += encrypt(blockId + i, *key1) * encrypt(blockId + i, *key2);
		/* buf[localId] += 1; */
	}

	int localId = get_local_id(0);
	buf[localId] = sum;

	barrier(CLK_LOCAL_MEM_FENCE);
	for (int i = LOCALSIZE >> 1; i; i >>= 1) {
		if (localId < i) {
			buf[localId] += buf[localId + i];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}

	int groupId = get_group_id(0);
	if (localId == 0) {
		ans[groupId] = buf[0];
	}
}
