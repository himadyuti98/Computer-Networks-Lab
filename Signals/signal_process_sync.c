#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<signal.h>
#include<sys/wait.h>

static int *x,*y; 
pid_t p1,p2;

void fn1()
{
	*x=*y+1;
	printf("x=%d y=%d\n",*x,*y);
	sleep(1);
	kill(p2,SIGUSR2);
}
void fn2()
{
	*y=*x+1;
	printf("x=%d y=%d\n",*x,*y);
	sleep(1);
	
	kill(p1,SIGUSR1);
}

int main()
{

	signal(SIGUSR1,fn1);
	signal(SIGUSR2,fn2);
	
	x=mmap(NULL,sizeof *x,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,-1,0); 
	*x = 5;	

	y=mmap(NULL,sizeof *y,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,-1,0);
	*y = 7;	

	printf("x=%d y=%d\n",*x,*y);
	p1=getpid();
	p2=fork();
			
	if(p2>0)
	{
		//kill(p2,SIGUSR2);
		wait(NULL);
		printf("**********************************\n");	
	}
	else
	{
		kill(p1,SIGUSR1);
		while(1);
	}
	return 0;
	
}
