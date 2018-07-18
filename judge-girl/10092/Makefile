CC = gcc
CFLAGS = -std=c99
LINKOPT = -lOpenCL

all: main

%-cl: %.c
	$(CC) $(CFLAGS) $< $(LINKOPT) -o $@
