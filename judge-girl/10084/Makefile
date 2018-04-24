1: 1.c secret.c utils.h
	gcc -std=c99 -O2 -pthread 1.c secret.c -o 1
prefixsum-seq: prefixsum-seq.c secret.c utils.h
	gcc -std=c99 -O2 -pthread prefixsum-seq.c secret.c -o prefixsum-seq

run1: 1
	./1
run2: prefixsum-seq
	./prefixsum-seq
