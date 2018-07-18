#define uint32_t unsigned int

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
					  int startId,
					  int N) {
	__local uint32_t buf[LOCALSIZE];

	int globalId = get_global_id(0);

	int blockId = (globalId + startId) * SEQSIZE;
	int end = blockId + SEQSIZE;
	end = (end < N)? end : N;

	uint32_t sum = 0;
	for (int i = blockId; i < end; ++i) {
		sum += encrypt(i, key1) * encrypt(i, key2);
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
