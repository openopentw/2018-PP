CFILE = 1.cpp
CFLAGS = 

all: ${CFILE}
	gcc -std=c++98 -O3 ${CFILE} -o a-uni.out ${CFLAGS}
	gcc -std=c++98 -fopenmp -O3 ${CFILE} -o a.out ${CFLAGS}

.PHONY: run, run-uni

run:
	./a.out

run-uni:
	./a-uni.out
