CFILE = 2.c
CFLAGS = 

all: ${CFILE}
	gcc -std=c99 -O3 ${CFILE} -o a-uni.out ${CFLAGS}
	gcc -std=c99 -fopenmp -O3 ${CFILE} -o a.out ${CFLAGS}

.PHONY: run, run-uni

run:
	./a.out

run-uni:
	./a-uni.out
