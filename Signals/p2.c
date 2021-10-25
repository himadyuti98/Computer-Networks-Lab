#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<stdlib.h>


int main(int argc, char* argv[])
{
	printf("process2\n");
	printf("%d",getppid());
	kill(getppid(),SIGUSR2);
	return 0;
}
