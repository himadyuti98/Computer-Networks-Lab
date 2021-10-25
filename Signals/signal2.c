#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>

void fromp1()
{
	printf("Recieving signal from process1\n");
}
void fromp2()
{
	printf("Recieving signal from process2\n");
}

int main()
{
	signal(SIGUSR1,fromp1);
	signal(SIGUSR2,fromp2);
	int c = fork();
	if(c>0)
	{
		printf("process1 %d\n",getpid());
		kill(c,SIGUSR1);
		wait(NULL);
		printf("*****************\n");
	}
	else
	{
		printf("1\n");
		char * a[]={NULL};
		execvp("./p2",a);
	}
	return 0;
}
