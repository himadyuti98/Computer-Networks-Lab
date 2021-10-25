#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<stdlib.h>

int cosfd,clsfd;

void * recvco(void * f)
{
	char buff[40];
	while(1)
	{
		read(cosfd,buff,40);
		printf("%s\n",buff);
		sleep(1);
	}
}

void * recvcl(void * f)
{

}

void * sndcl(void * f)
{

}

int main()
{
	cosfd=socket(AF_INET, SOCK_STREAM, 0);
	clsfd=socket(AF_INET, SOCK_DGRAM, 0);
	int port;
	scanf("%d",&port);
	struct sockaddr_in saddr,saddr1;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(5050);
	saddr1.sin_family=AF_INET;
	saddr1.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr1.sin_port = htons(port);
	char buffer[20];
	sprintf(buffer,"%d",port);
	sendto(clsfd,buffer,strlen(buffer),0,(struct sockaddr*)&saddr, sizeof(saddr));
	connect(cosfd,(struct sockaddr*)&saddr1, sizeof(saddr1));
	pthread_t co,rcl,scl;
	pthread_create(&co,NULL,&recvco,NULL);
	pthread_create(&rcl,NULL,&recvcl,NULL);
	pthread_create(&scl,NULL,&sndcl,NULL);
	pthread_join(co,NULL);
	pthread_join(rcl,NULL);
	pthread_join(scl,NULL);
}
