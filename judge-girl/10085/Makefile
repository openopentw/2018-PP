CFLAG=-std=c99 -pthread

all: main.c sum.c job.c
	gcc $(CFLAG) main.c -c
	gcc $(CFLAG) sum.c -c
	gcc $(CFLAG) main.o sum.o job.c -o job

.PHONY: run
run: job
	./job
