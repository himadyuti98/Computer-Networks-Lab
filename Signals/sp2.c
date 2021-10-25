#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/mman.h>
#include<semaphore.h>
#include<fcntl.h>


static int *x,*y; 
sem_t *s1,*s2;

void fromp1()
{
	printf("Recieving signal from process1\n");
}
void fromp2()
{
	printf("Recieving signal from process2\n");
}

int main(int argc, char* argv[])
{
	sem_unlink("p1sem");
	sem_unlink("p2sem");
	shm_unlink("x");
	shm_unlink("y");
	signal(SIGUSR1,fromp1);
	signal(SIGUSR2,fromp2);
	
	int shm_fd1=shm_open("x",O_RDWR,0666);
	int shm_fd2=shm_open("y",O_RDWR,0666);

	x=mmap(NULL,sizeof *x,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,shm_fd1,0); 
	
	y=mmap(NULL,sizeof *y,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,shm_fd2,0);
	
	s1 = sem_open("p1sem",O_CREAT|O_EXCL,0644,0);
	s2 = sem_open("p2sem",O_CREAT|O_EXCL,0644,0);
	if(s1==SEM_FAILED || s2==SEM_FAILED)
		printf("failed2");
	*y=getpid();
	printf("$$$$$$$$$$$$$$$$$$$$$$\n");
	sem_post(s2);
	printf("$$$$$$$$$$$$$$$$$$$$$$\n");
	sem_wait(s1);
	printf("$$$$$$$$$$$$$$$$$$$$$$\n");
	int p = *x;

	kill(p,SIGUSR1);

	return 0;
}
