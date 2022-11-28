#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

#define MAX_THREAD_NUM (10000)

pthread_t dudes[MAX_THREAD_NUM];

struct timeval stop, start;

static void inline init()
{
}

static void inline gettime(int i, struct timeval *start, struct timeval *stop)
{
	printf("count %d, took %lu us\n", i,
	       (stop->tv_sec - start->tv_sec) * 1000000 + stop->tv_usec - start->tv_usec);
}

static void inline time_hook(int i)
{
		if (i == 0)
			gettimeofday(&start, NULL);

		if (i % 100 == 0) {
			gettimeofday(&stop, NULL);
			gettime(i, &start, &stop);
		}
}

void *dude_task()
{
	printf("dude create\n");
	while (1) {
	}
}

int main()
{
	int i;
	int thread_remain = MAX_THREAD_NUM;

	init();

	for (i = 0; i < thread_remain; ++i) {
		time_hook(i);
		pthread_create(&dudes[i], NULL, dude_task, NULL);
	}


	for (i = 0; i < thread_remain; ++i)
		pthread_join(dudes[i], NULL);

	return 0;
}
