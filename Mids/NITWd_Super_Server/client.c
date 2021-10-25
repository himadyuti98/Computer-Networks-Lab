#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>

int sfd;
char buff[50];

int main()
{
	int port;
	scanf("%d",&port);
	sfd= socket(AF_INET, SOCK_STREAM, 0);
	printf("%d\n", sfd);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(port);
	
	connect(sfd,(struct sockaddr*)&saddr, sizeof(saddr));

	strcpy(buff,"hi, i am client");
	write(sfd,buff,strlen(buff));
	memset(buff,'\0',50);
	read(sfd,buff,50);
	printf("%s\n", buff);
	
}