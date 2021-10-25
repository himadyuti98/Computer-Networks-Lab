#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/un.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>

struct sockaddr_in server;

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t servlen=sizeof(server);
	server.sin_port = atoi(argv[1]);
	int f = connect(sfd,(struct sockaddr*)&server, sizeof(server));
	if(f<0)
		perror("connect");
	char buff[50];
	while(1)
	{
		memset(buff,'\0',50);
		scanf("%s",buff);
		send(sfd,buff,strlen(buff),0);
		perror("send");
	}
}