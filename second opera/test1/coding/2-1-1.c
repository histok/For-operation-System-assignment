#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>

int flag=0;
void inter_handler(int signum){
	if(signum == SIGALRM ||signum ==SIGQUIT||signum== SIGUSR1||signum==SIGUSR2){
		flag =1;
	}
	else{
		printf("[info] error \n");
	}
}

void waiting(int i){
	while(!flag){
		sleep(1);
	}
	printf("Child process%d is killed by parent!!\n",i);
	flag=0;
}

int main(){
	int timeout =0;
	int flag_child1=0, flag_child2=0;
	pid_t pid1=-1, pid2=-1;
	while(pid1==-1)pid1=fork();
	if(pid1 >0){
		while(pid2==-1)pid2=fork();
		if(pid2>0){
			int time=0;
			alarm(5);
			while(1){
				signal(SIGALRM, SIG_IGN);
				signal(SIGQUIT, inter_handler);
				printf("[info] time passed: %d\n",++time);
				if(flag||timeout){

					kill(pid1,SIGUSR1);
					kill(pid2,SIGUSR2);
					wait(NULL);
					wait(NULL);

					printf("[info] Parent process is killed!!\n");
					exit(0);
				}
				if(time==4){
					timeout =1;
				}
				sleep(1);
			}

		}else{
			signal(SIGUSR2, inter_handler);
			signal(SIGQUIT,SIG_IGN);
			signal(SIGALRM,SIG_IGN);
			waiting(2);
			return 0;
		}
	}
	else {
		signal(SIGUSR1, inter_handler);
		signal(SIGQUIT,SIG_IGN);
		signal(SIGALRM,SIG_IGN);
		waiting(1);
		return 0;
	}
	return 0;
}
