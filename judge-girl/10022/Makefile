matrix.out: matrix.h matrix.c main.c
	gcc -std=c99 -fopenmp -O2 matrix.h matrix.c main.c -o matrix.out

.PHONY: run

run:
	./matrix.out
