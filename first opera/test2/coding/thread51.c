#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
void *thread_func(void *params){
	long tid =(long)params;
	printf("Thread%ld create success!\n", tid);
	printf("Thread%ld tid =%ld, pid =%d\n", tid,(long)pthread_self(),getpid());
	pthread_exit(NULL);
}
int main(int argc, char *argv[]){
	if(argc !=2){
		fprintf(stderr,"Usage: %s <thread_count>\n",argv[0]);
		return 1;
	}
	int thread_count =atoi(argv[1]);
	pthread_t threads[thread_count];

	for(long i=0;i<thread_count;i++){
		pthread_create(&threads[i],NULL,thread_func, (void*)i);
	}
	for(int i=0;i<thread_count;i++){
		pthread_join(threads[i],NULL);
	}
	return 0;
}
