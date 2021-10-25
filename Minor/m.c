#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int sfd,nsfd,ssfd;

int main(int argc, char* argv[])
{
	sfd = socket(AF_INET,SOCK_STREAM,0);
	nsfd = socket(AF_INET,SOCK_STREAM,0);
	ssfd = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in saddr,caddr;

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(atoi(argv[1]));
	while(bind(sfd,(struct sockaddr*)&saddr,sizeof(saddr))<0);
	printf("bind done\n");
	listen(sfd,SOMAXCONN);
	perror("listen");

	while(1)
	{
		socklen_t l;
		nsfd = accept(sfd,(struct sockaddr*)&caddr,&l);
		char ad[20];
		inet_ntop(AF_INET,&(caddr.sin_addr),ad,20);
		printf("Client addr%s\n",ad );
		int p=(int)ntohs(caddr.sin_port);
		printf("%d\n",p);
		int k;
		read(nsfd,(int*)&k,sizeof(int));
		printf("received %d\n",k );
		saddr.sin_port = htons(atoi(argv[1+k]));
		connect(ssfd,(struct sockaddr*)&saddr,sizeof(saddr));
		write(ssfd,(struct sockaddr*)&caddr,l);
		close(nsfd);
		close(ssfd);
	}

	return 0;
}