#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

void fromp()
{
	printf("Recieving signal from parent\n");
}
void fromc()
{
	printf("Recieving signal from child\n");
}

int main()
{
	signal(SIGUSR1,fromp);
	signal(SIGUSR2,fromc);
	int c = fork();
	if(c>0)
	{
		//sleep(1);
		printf("parent\n");
		kill(c,SIGUSR1);
		wait(NULL);
	}
	else
	{
		//sleep(5);
		printf("child\n");
		kill(getppid(),SIGUSR2);
	}
	return 0;
}
