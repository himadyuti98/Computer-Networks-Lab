#include<stdio.h>	//
#include<unistd.h>      //fork , exec
#include<sys/types.h>   //pid_t
#include<sys/wait.h>	//wait
#include<stdlib.h>	//exit
int main()
{

	printf("1\n");
	pid_t c ;
	c= fork();
	if(c>0)
	{
		printf("%d\n",c);
		printf("2\n");
		int x;
		int t=wait(&x);
		printf("%d %d\n",t,x);
		printf("3\n");
	}	
	else
	{
		printf("4\n");
		char *a[]={NULL};
				
		execvp("./exec1",a);
		printf("5\n");

	}

}
