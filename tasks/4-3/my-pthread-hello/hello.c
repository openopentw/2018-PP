#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

const int NUM_THREADS = 10;

void *printHello(void *threadId) {
	int id = *(int *)(threadId);
	printf("%d\n", id);
}

int main() {
	pthread_t threads[NUM_THREADS];
	int threadId[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; ++i) {
		threadId[i] = i;
		int rc = pthread_create(&threads[i], NULL,
								printHello, (void *)&threadId[i]);
		if (rc) {
			printf("main: error code %d\n", rc);
			exit(-1);
		}
	}
	/* pthread_exit(NULL); */
	return 0;
}
