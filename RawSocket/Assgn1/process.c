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

struct sockaddr_in server,client;

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.2");
	bind(sfd, (struct sockaddr*)&server, sizeof(server));
	perror("bind");
	//listen(sfd,SOMAXCONN);
	//perror("listen");
	//int nsfd = accept(sfd,NULL,NULL);
	int nsfd = sfd;
	socklen_t clilen=sizeof(client);
	if(nsfd>0)
	{
		printf("accepted\n");
		char buff[50];
		while(1)
		{
			//read(nsfd,buff,50);
			int s = recvfrom(nsfd,buff,50,MSG_WAITALL,(struct sockaddr*)&client,(socklen_t*)&clilen);
			if(s>0)
			{
				struct iphdr *ip;
				  ip=(struct iphdr*)buff;
				  int x = (ip->ihl)*4;
				printf("%s\n", buff+x);
			}
		}
	}
	return 0;
}