#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int numsum=0;
void *ad(void *);
void *de(void *);
int main()
{
	pthread_t id1,id2;
	pthread_create(&id1,NULL,ad,NULL);
	pthread_create(&id2,NULL,de,NULL);
	
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);

	printf("numsum = %d\n",numsum);
}
void *ad(void *params){
	for(int i=0; i<100000; i++){
		numsum=numsum+100;
	}
	printf("adding complete\n");

	pthread_exit(NULL);
}
void *de(void *params){
	for(int i=0;i<100000;i++){
		numsum=numsum-100;
	}
	printf("de complete\n");
	pthread_exit(NULL);
}
