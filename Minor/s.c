#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<pthread.h>

int sfd,nsfd;
int ssfd[20];
int n=0;
int port = 6060;

void * service(void * p)
{
	int sfd = *(int*)p;
	char buff[50];
	strcpy(buff,"service\n");
	printf("%s\n", buff);
	while(1)
	{
		write(sfd,buff,strlen(buff));
	}
}

int main(int argc, char * argv[])
{
	sfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(atoi(argv[1]));
	bind(sfd,(struct sockaddr*)&saddr,sizeof(saddr));
	listen(sfd,20);
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		struct sockaddr_in caddr;
		//fflush((FILE*)nsfd);
		read(nsfd,(struct sockaddr*)&caddr,sizeof(caddr));
		/*char ad[20];
		inet_ntop(AF_INET,&(caddr.sin_addr),ad,20);
		printf("Client addr%s\n",ad );
		int p=(int)ntohs(caddr.sin_port);
		printf("%d\n",p);*/
		ssfd[n] = socket(AF_INET,SOCK_STREAM,0);
		saddr.sin_port = htons(port++);
		bind(ssfd[n],(struct sockaddr*)&saddr,sizeof(saddr));
		if(connect(ssfd[n],(struct sockaddr*)&caddr,sizeof(caddr))<0) perror("connect");
		pthread_t pid;
		pthread_create(&pid,NULL,service,(void*)&ssfd[n]);
		close(nsfd);
	}
}