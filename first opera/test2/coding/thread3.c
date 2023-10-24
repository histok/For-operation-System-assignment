#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int numsum =0;
pthread_mutex_t numsum_mutex =PTHREAD_MUTEX_INITIALIZER;

void *ad(void *params){
	for(int i=0;i<100000;i++){
		pthread_mutex_lock(&numsum_mutex);
		numsum =numsum+100;
		pthread_mutex_unlock(&numsum_mutex);
	}
	printf("adding complete\n");
}

void *de(void *params){
	for(int i=0;i<100000;i++){
		pthread_mutex_lock(&numsum_mutex);
		numsum =numsum -100;
		pthread_mutex_unlock(&numsum_mutex);
	}
	printf("de complete\n");
}

int main(){
	pthread_t id1,id2;
	pthread_create(&id1,NULL,ad,NULL);
	pthread_create(&id2,NULL,de,NULL);

	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	printf("numsum =%d \n",numsum);

	pthread_mutex_destroy(&numsum_mutex);
}
