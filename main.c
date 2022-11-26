#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>

pthread_t dudeA, dudeB;
pthread_mutex_t speak_lock;
pthread_cond_t speak;

#define A_SPEAKING (1)
#define B_SPEAKING (2)

int dude_speaking = A_SPEAKING;

static void inline init()
{
	pthread_mutex_init(&speak_lock, NULL);
	pthread_cond_init(&speak, NULL);
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
