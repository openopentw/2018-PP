CC = g++
CFLAGS = -std=c++98 -O2
CFILE = sequence.cpp

all: $(CFILE)
	$(CC) -fopenmp $(CFLAGS) $< -lm -o a.out
	$(CC) $(CFLAGS) $< -lm -o a-uni.out
