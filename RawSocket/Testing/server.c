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

int main(int argc, char * argv[])
{
	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	perror("socket");
	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL
	
		struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

		char buff[]="hello";	
	
	unsigned int client_len=sizeof(client);
	
  sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
  perror("send");
  while(1);
}
