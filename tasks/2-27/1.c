#include <stdio.h>
#include <stdlib.h>	// rand
#include <time.h>	// time

typedef struct particle {
	double mass;
	double pos[3];
	double acc[3];
} Particle;

double d_square(double d) {
	return d * d;
}

double d_rand() {
	return (double)rand() / RAND_MAX;
}

double get_acc(Particle *p) {
	return p->mass / (d_square(p->pos[0]) + d_square(p->pos[1]) + d_square(p->pos[2]));
}

int main()
{
	srand (time(NULL));
	int N = 20000;
	printf("N = %d\n", N);
	int t = 1;

	Particle p[N];
	for(int i = 0; i < N; ++i) {
		p[i].mass = d_rand();
		for(int j = 0; j < 3; ++j) {
			p[i].pos[j] = d_rand();
			p[i].acc[j] = d_rand();
		}
	}

	double acc[N][3];
	for(int k = 0; k < N; ++k) {
		for(int i = 0; i < 3; ++i) {
			acc[k][i] = 0;
			for(int j = 0; j < N; ++j) {
				if(j == i) continue;
				acc[k][i] += get_acc(&(p[j])) * d_square(p[k].pos[i] - p[j].pos[i]);
			}
		}
	}

	for(int i = 0; i < N; ++i) {
		for(int j = 0; j < 3; ++j) {
			p[i].pos[j] += 0.5 * acc[i][j] * t * t;
		}
	}

	return 0;
}
