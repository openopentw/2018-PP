CC = g++
CFLAGS = -std=c++98 -O3

all: life.cpp
	$(CC) -fopenmp $(CFLAGS) $< -lm -o life.out
	$(CC) $(CFLAGS) $< -lm -o life-uni.out
