#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>

int numsum =0;
sem_t mutex;

void *ad(void *params){
	for(int i=0;i<100000;i++){
		sem_wait(&mutex);
		numsum =numsum+100;
		sem_post(&mutex);
	}
	printf("adding complete\n");
	pthread_exit(NULL);
}

void *de(void *params){
	for(int i=0;i<100000;i++){
		sem_wait(&mutex);
		numsum =numsum -100;
		sem_post(&mutex);
	}
	printf("de complete\n");
	pthread_exit(NULL);
}

int main(){
	pthread_t id1,id2;
	sem_init(&mutex,0,1);
	pthread_create(&id1,NULL,ad,NULL);
	pthread_create(&id2,NULL,de,NULL);

	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	printf("numsum =%d \n",numsum);

	sem_destroy(&mutex);
}
