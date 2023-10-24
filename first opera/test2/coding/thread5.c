#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
	int thread_count =2;
	char command[100];

	for(int i=1; i<=thread_count;i++){
		sprintf(command,"./thread_program %d",i);
		system(command);
	}

	system("ps -ef");

	return 0;
}
