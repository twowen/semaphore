/*
 * Owen Wu <twowen@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_NUM_THREAD 2

sem_t binSem;

#if 0
#define THREAD(x) thd ## x
#endif

void *thd1(void *arg)
{
	int i;
	static char msg[512];

	for (i = 0; i < 1; i++) {
		printf("[%d] %s\n", (i + 1), "thread 1 is waiting");
		sem_wait(&binSem);
		
		printf("[%d] %s\n", (i + 1), "thread 1 got a semaphore, then running");
		sleep(1);
	}

	sprintf(msg, "%s", "thread 1 finished!");

	pthread_exit((void *)msg);
}

void *thd2(void *arg)
{
	int i;
	static char msg[512];

	for (i = 0; i < 3; i++) {
		printf("[%d] %s\n", (i + 1), "thread 2 is waiting");
		sem_wait(&binSem);
		
		printf("[%d] %s\n", (i + 1), "thread 2 got a semaphore, then running");
		sleep(1);
	}

	sprintf(msg, "%s", "thread 2 finished!");

	pthread_exit((void *)msg);
}

void main(void)
{
	int i, ret, cnt;
	pthread_t thds[MAX_NUM_THREAD];
	void (*thd[MAX_NUM_THREAD])(void *) = {thd1, thd2};
	void *thdResult;

	ret = sem_init(&binSem, 0, 0);
	if (ret) {
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < MAX_NUM_THREAD; i++) {
#if 0
		ret = pthread_create(&thds[i], NULL, THREAD(i), NULL);
#else
		ret = pthread_create(&thds[i], NULL, (void *)thd[i], NULL);
#endif
		if (ret) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
		printf("Create thread %d done\n", (i + 1));
	}

	cnt = 6;
	while (cnt--) {
		sem_post(&binSem);
		printf("%s\n", "In main(), sleep for 1 second");
		sleep(1);
	}

	for (i = 0; i < MAX_NUM_THREAD; i++) {
#if 0
		ret = pthread_join(THREAD(i), &thdResult);
#else
		ret = pthread_join(thds[i], &thdResult);
#endif
		if (ret) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
		printf("[%d] %s\n", i, (char *)thdResult);
	}

	sem_destroy(&binSem);

	exit(EXIT_SUCCESS);
}
