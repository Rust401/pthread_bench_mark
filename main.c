#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>

pthread_t dudeA, dudeB;
pthread_mutex_t speak_lock;
pthread_mutex_t fuck_lock;
pthread_cond_t speak;
pthread_cond_t fuck;

int dude_speaking = 1;
int dude_fucking = 2;

static void inline init()
{
	pthread_mutex_init(&speak_lock, NULL);
	pthread_mutex_init(&fuck_lock, NULL);
	pthread_cond_init(&speak, NULL);
	pthread_cond_init(&fuck, NULL);
}

void *dudeA_task()
{
	while (1) {
		pthread_mutex_lock(&speak_lock);

		while (dude_speaking != 1)
			pthread_cond_wait(&speak, &speak_lock);

		printf("A: speaking\n");

		pthread_mutex_unlock(&speak_lock);


		pthread_mutex_lock(&fuck_lock);

		dude_fucking = 1;
		printf("A: fucking\n");
		pthread_cond_signal(&fuck);
		dude_fucking = 2;

		pthread_mutex_unlock(&fuck_lock);
	}
}

void *dudeB_task()
{
	while (1) {
		pthread_mutex_lock(&fuck_lock);

		while (dude_fucking != 2)
			pthread_cond_wait(&fuck, &fuck_lock);

		printf("B: fucking\n");

		pthread_mutex_unlock(&fuck_lock);


		pthread_mutex_lock(&speak_lock);

		dude_speaking = 2;
		printf("B: speaking\n");
		pthread_cond_signal(&speak);
		dude_speaking = 1;

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
