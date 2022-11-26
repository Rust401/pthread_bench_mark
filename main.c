#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>

pthread_t dudeA, dudeB;
pthread_mutex_t speak_lock;
pthread_cond_t speak;

int dude_speaking = 1;

static void inline init()
{
	pthread_mutex_init(&speak_lock, NULL);
	pthread_cond_init(&speak, NULL);
}

void *dudeA_task()
{
	while (1) {
		pthread_mutex_lock(&speak_lock);
		while (dude_speaking != 1)
			pthread_cond_wait(&speak, &speak_lock);

		/*
		 * dudeA could do sth here
		 */
		printf("dudeA speaking\n");
		pthread_mutex_unlock(&speak_lock);
	}
}

void *dudeB_task()
{
	while (1) {
		pthread_mutex_lock(&speak_lock);
		dude_speaking = 2;
		/*
		 * dudeB could do sth here
		 */
		printf("dudeB speaking\n");
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
