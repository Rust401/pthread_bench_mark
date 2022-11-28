#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

#define A_SPEAKING (1)
#define B_SPEAKING (2)

pthread_t dudeA, dudeB;
pthread_mutex_t speak_lock;
pthread_cond_t speak;
struct timeval stop, start;

int dude_speaking = A_SPEAKING;
uint64_t wakecount = 0;

static void inline init()
{
	pthread_mutex_init(&speak_lock, NULL);
	pthread_cond_init(&speak, NULL);
}

static void inline gettime(struct timeval *start, struct timeval *stop)
{
	printf("count %lu, took %lu us\n", wakecount,
	       (stop->tv_sec - start->tv_sec) * 1000000 + stop->tv_usec - start->tv_usec);
}

static void inline time_hook()
{
		if (wakecount == 0)
			gettimeofday(&start, NULL);

		if (wakecount % 1000 == 0) {
			gettimeofday(&stop, NULL);
			gettime(&start, &stop);
		}
}

void *dudeA_task()
{
	while (1) {
		pthread_mutex_lock(&speak_lock);

		/* wait B */
		while (dude_speaking != A_SPEAKING)
			pthread_cond_wait(&speak, &speak_lock);

		/* A do sth */
		dude_speaking = A_SPEAKING;

		time_hook();

		++wakecount;
		printf("A: speaking\n");
		dude_speaking = B_SPEAKING;

		/* signal B */
		pthread_cond_signal(&speak);

		pthread_mutex_unlock(&speak_lock);
	}
}

void *dudeB_task()
{
	while (1) {
		pthread_mutex_lock(&speak_lock);

		/* wait A */
		while (dude_speaking != B_SPEAKING)
			pthread_cond_wait(&speak, &speak_lock);

		/* B do sth */
		dude_speaking = B_SPEAKING;

		time_hook();

		++wakecount;

		/* wait A */
		while (dude_speaking != B_SPEAKING)
			pthread_cond_wait(&speak, &speak_lock);

		/* B do sth */
		dude_speaking = B_SPEAKING;
		printf("B: speaking\n");
		dude_speaking = A_SPEAKING;

		/* signal A */
		pthread_cond_signal(&speak);
		pthread_mutex_unlock(&speak_lock);
	}
}

int main()
{
	init();

	printf("inited\n");

	pthread_create(&dudeA, NULL, dudeA_task, NULL);
	pthread_create(&dudeB, NULL, dudeB_task, NULL);

	pthread_join(dudeA, NULL);
	pthread_join(dudeB, NULL);

	return 0;
}
