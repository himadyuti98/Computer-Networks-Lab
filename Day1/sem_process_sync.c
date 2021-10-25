#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<semaphore.h>

static int *x,*y; 
sem_t *s1,*s2;   

int main()
{


	x=mmap(NULL,sizeof *x,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,-1,0); 
	//map anon to ignore fd
	*x = 5;	

	y=mmap(NULL,sizeof *y,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,-1,0);
	*y = 7;	
	
  

	s1 = sem_open("sem1",O_CREAT,0644,0); //share semaphores among processes
	s2 = sem_open("sem2",O_CREAT,0644,1);
	// sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);

	printf("x=%d y=%d\n",*x,*y);
	pid_t p=fork();
			
	if(p>0)
	{
		while(1)
		{
			sem_wait(s2);
			*x=*y+1;
			printf("x=%d y=%d\n",*x,*y);
			sem_post(s1);
			sleep(1);
		}	
	}
	else
	{
		while(1)
		{
			
			sem_wait(s1);
			*y=*x+1;
			printf("x=%d y=%d\n",*x,*y);
			sem_post(s2);
			sleep(1);
		}	
	}
	return 0;
	
}
