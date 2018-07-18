#define uint32_t unsigned int

/* #define MOD2 1 */

#define LOCALSIZE 1024
#define SEQSIZE 64

static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
	return  (x << n) | (x >> (32-n));
}

static inline uint32_t encrypt(uint32_t m, uint32_t key) {
	return (rotate_left(m, key&31) + key)^key;
}

__kernel void vecDot (__global uint32_t* ans,
					  uint32_t key1,
					  uint32_t key2,
					  int startId) {
	__local uint32_t buf[LOCALSIZE];

	int globalId = get_global_id(0);
#ifdef MOD2
	int blockId = 2 * globalId * SEQSIZE + startId;
#else
	int blockId = (globalId + startId) * SEQSIZE;
#endif

	uint32_t sum = 0;
#ifdef MOD2
	int endId = 2 * SEQSIZE;
	for (int i = 0; i < endId; i += 2) {
#else
	for (int i = 0; i < SEQSIZE; ++i) {
#endif
		sum += encrypt(blockId + i, key1) * encrypt(blockId + i, key2);
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
