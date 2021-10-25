#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<semaphore.h>

int main(int argc, char const *argv[])
{
	sem_t *sem1, *sem2;
	int shmid = shmget(1000,2*sizeof(sem_t)+2*sizeof(int),0666|IPC_CREAT);
	int *x, *y;
	sem1 = (sem_t *) shmat(shmid,0,0);
	sem2 = (void *)sem1 + sizeof(sem_t);
	x = (void *)sem2 + sizeof(sem_t);
	y = (void *)x + sizeof(int);
	sem_init(sem1,1,0);
	sem_init(sem2,1,1);
	pid_t c = fork();
	if(c==0)
	{
		while(1)
		{
			
			sem_wait(sem1);
			*y=*x+1;
			printf("child increased y\nx=%d y=%d\n",*x,*y);
			sem_post(sem2);
			sleep(1);
		}	
	}
	else
	{
		while(1)
		{
			sem_wait(sem2);
			*x=*y+1;
			printf("parent increased x\nx=%d y=%d\n",*x,*y);
			sem_post(sem1);
			sleep(1);
		}
	}
	return 0;
}