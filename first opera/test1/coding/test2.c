#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
	pid_t pid,pid1;
	pid=fork();
	
	if(pid<0)
	{
		fprintf(stderr,"Fork Failed");
		return 1;
	}
	else if(pid==0)
	{
		pid1=getpid();
		printf("child1 process PID: %d\n",pid1);
		execl("./b.out",NULL);
		printf("child2 process PID: %d\n",pid1);
	}
	else
	
	{
		pid1=getpid();
		printf("parent process PID: %d\n",pid1);
		
	}
	return 0;
}

