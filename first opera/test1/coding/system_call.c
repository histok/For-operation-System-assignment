#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	pid_t pid =getpid();
	printf("child:child: pid=%d\n",pid);

}
