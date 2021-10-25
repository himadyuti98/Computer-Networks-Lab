#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<stdlib.h>

int cosfd,clsfd,clsfd2,port;
struct sockaddr_in saddr,saddr1,caddr;
pthread_t co,rcl,scl;

	
void * recvco(void * f)
{
	printf("cl started\n");
	char buff[40];
	while(1)
	{
		int s = read(cosfd,buff,40);
		if(s>0)
			printf("%s\n",buff);
		sleep(1);
	}
}

void * recvcl(void * f)
{
	/*char ad[20];
	inet_ntop(AF_INET,&(caddr.sin_addr),ad,20);
	int p=(int)ntohs(caddr.sin_port);
	printf("Client addr%s %d\n",ad,p);
	*/char b[40];
	socklen_t l = (socklen_t)sizeof(saddr);
	while(1){
	/*inet_ntop(AF_INET,&(caddr.sin_addr),ad,20);
	p=(int)ntohs(caddr.sin_port);
	printf("Client addr%s %d\n",ad,p);
	*/int s = recvfrom(clsfd,b,40,0,(struct sockaddr*)&saddr,&l);
	if(s>0)
	{
		printf("%s\n",b);
		
	
	}
	printf("enter port\n");
	//scanf("%d",&port);
	
	
	
	}
}

void * sndcl(void * f)
{
	char b[40];
	while(1){
	scanf("%s",b);
	printf("hihi\n");
	if(b[0]>='0' && b[0]<='9')
	{
		close(cosfd);
		cosfd=socket(AF_INET, SOCK_STREAM, 0);
		saddr1.sin_port = htons(atoi(b));
		char buffer[20];
		sprintf(buffer,"%d",atoi(b));
		saddr1.sin_family=AF_INET;
		saddr1.sin_addr.s_addr = inet_addr("127.0.0.1");
		sendto(clsfd2,buffer,strlen(buffer),0,(struct sockaddr*)&saddr1, sizeof(saddr1));
		char ad[20];
		inet_ntop(AF_INET,&(saddr1.sin_addr),ad,20);
		int p=(int)ntohs(saddr1.sin_port);
		printf("Client addr%s %d\n",ad,p);
		printf("sent\n");
		//printf("%d\n",connect(cosfd,(struct sockaddr*)&saddr1, sizeof(saddr1)));
		while(connect(cosfd,(struct sockaddr*)&saddr1, sizeof(saddr1))==-1);
		pthread_create(&co,NULL,&recvco,NULL);
	}
	else
		sendto(clsfd,b,strlen(b),0,(struct sockaddr*)&saddr,sizeof(saddr));}

}

int main()
{
	cosfd=socket(AF_INET, SOCK_STREAM, 0);
	clsfd=socket(AF_INET, SOCK_DGRAM, 0);
	clsfd2=socket(AF_INET, SOCK_DGRAM, 0);
	
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(5050);
	caddr.sin_family=AF_INET;
	caddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	caddr.sin_port = htons(4050);
	int f = bind(clsfd,(struct sockaddr*)&caddr,sizeof(caddr));
	printf("f=%d\n",f);
	pthread_create(&rcl,NULL,&recvcl,NULL);
	pthread_create(&scl,NULL,&sndcl,NULL);
	printf("heloooooooooooo\n");
	/*scanf("%d",&port);
	saddr1.sin_port = htons(port);
		char buffer[20];
		sprintf(buffer,"%d",port);
		saddr1.sin_family=AF_INET;
		saddr1.sin_addr.s_addr = inet_addr("127.0.0.1");
		sendto(clsfd2,buffer,strlen(buffer),0,(struct sockaddr*)&saddr1, sizeof(saddr1));
		//printf("%d\n",connect(cosfd,(struct sockaddr*)&saddr1, sizeof(saddr1)));
		while(connect(cosfd,(struct sockaddr*)&saddr1, sizeof(saddr1))==-1);
		pthread_create(&co,NULL,&recvco,NULL);
	pthread_join(co,NULL);*/
	pthread_join(rcl,NULL);
	pthread_join(scl,NULL);
}
