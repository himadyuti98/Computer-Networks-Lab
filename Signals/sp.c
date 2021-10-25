#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	int c = fork();
	if(c>0)
	{
		char * a[]={NULL};
		execvp("./sp1",a);
		wait(NULL);
	}
	else
	{
		char * a[]={NULL};
		execvp("./sp2",a);
	}
	return 0;
}
