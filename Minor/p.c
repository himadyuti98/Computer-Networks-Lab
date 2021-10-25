#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>

int sfd;

int main(int argc, char const *argv[])
{
	sfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in saddr,caddr;

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(atoi(argv[1]));

	connect(sfd,(struct sockaddr*)&saddr,sizeof(saddr));
	int k;
	printf("enter service number\n");
	scanf("%d",&k);
	write(sfd,&k,sizeof(int));
	socklen_t l = sizeof(caddr);
	getsockname(sfd,(struct sockaddr*)&caddr,&l);
	char ad[20];
	inet_ntop(AF_INET,&(caddr.sin_addr),ad,20);
	printf("Client addr%s\n",ad );
	int p=(int)ntohs(caddr.sin_port);
	printf("%d\n",p);
	int t = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(int));
	perror("setsockopt");
	shutdown(sfd,SHUT_RDWR);
	perror("shutdown");
	close(sfd);
	perror("close");
	sfd = socket(AF_INET,SOCK_STREAM,0);
	perror("socket");
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(int));
	perror("setsockopt");
	bind(sfd,(struct sockaddr*)&caddr,sizeof(caddr));
	perror("bind");
	listen(sfd,1);
	perror("listen");
	int nsfd = accept(sfd,NULL,NULL);
	perror("accept");
	char buff[30];
	while(1)
	{
		read(nsfd,buff,30);
		printf("%s",buff);
		memset(buff,'\0',30);
	}

	return 0;
}