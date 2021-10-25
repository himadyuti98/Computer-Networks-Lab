#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ipc.h>


int main(int argc, char *argv[])
{
	printf("hello\n");
	int nsfd = atoi(argv[1]);
	//printf("%d\n",nsfd);
	/*int nsfd = dup(0);
	int old_stdin = atoi(argv[1]);
	int old_stdout = atoi(argv[2]);
	dup2(old_stdin,0);
	dup2(old_stdout,1);*/
	char buff[50];
	read(nsfd,buff,50);
	printf("%s\n",buff);
	
	memset(buff,'\0',50);
	scanf("%s",buff);
	write(nsfd,buff,strlen(buff));
	mkfifo("./wk",IPC_CREAT|0666);
	int fd = open("./wk",O_RDWR);
	write(fd,argv[2],strlen(argv[2]));
	printf("sent %s\n", argv[2]);
	return 0;
}