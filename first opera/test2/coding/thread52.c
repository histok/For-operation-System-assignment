#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void *func(void *params){
	pthread_t tid =pthread_self();
	execlp("./b.out",NULL);
	printf("first tid= %d\n",tid);
	pthread_exit(0);
}
int main(){
	pid_t pid1,pid2;
	pid1=fork();
	if(pid1<0){
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if(pid1==0){
		pthread_t tid1;
		pthread_attr_t tid2;

		pthread_attr_init(&tid2);
		pthread_create(&tid1,&tid2,func,NULL);
		pthread_join(tid1,NULL);

		pid2=getpid();
		printf("child: pid1=%d\n", pid1);
		printf("child: pid2=%d\n", pid2);
	
	}
	else{
		pid2=getpid();
		printf("parent:pid1 =%d\n", pid1);
		printf("parent:pid2 =%d\n", pid2);
		wait(NULL);
	}
	return 0;

}
