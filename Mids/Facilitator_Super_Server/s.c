#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<stdlib.h>
#include<signal.h>

struct sockaddr_in saddr;
int nsfd,sfd;

void * runner(void * f)
{
	int nnsfd = *(int *)f;
	char buff[40];
	strcpy(buff,"providing service\n");
	//while(1)
	{
		printf("%s %d\n",buff,nsfd );
		write(nsfd,buff,strlen(buff));
	}
}

void handler(int signum)
{
	printf("handling sfd=%d\n",sfd);
	nsfd=accept(sfd,NULL,NULL);
	if(nsfd==-1)
		perror("");
	printf("accepted\n");
	pthread_t p;
	pthread_create(&p,NULL,&runner,&nsfd);
}

int main(int argc, char * argv[])
{
	printf("hello\n");
	signal(SIGUSR2,handler);
	printf("hello %d\n",getpid());
	sfd= socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(atoi(argv[0]));
	//printf("port=%d\n",atoi(argv[1]) );
	if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT, &(int){1},sizeof(int))<0)
	{
		perror("failed in s\n");
	}
	
	int f = bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	printf("s1f=%d\n",f);
	while(f==-1)
	{
		f = bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	}
	printf("s1f=%d\n",f);
	//setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR||SO_REUSEPORT, &(int){1},sizeof(int));
	listen(sfd,SOMAXCONN);
	while(1);
}
