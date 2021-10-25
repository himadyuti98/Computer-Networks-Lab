#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<sys/types.h>


int sfd,nsfd;
char buff[50],buffer[50];
struct sockaddr_in caddr,saddr;
char ad[20];

	
void * reader(void *f)
{
	while(1)
	{
		socklen_t d = (socklen_t)sizeof(caddr);	
		int s = recvfrom(sfd,buff,50,0,(struct sockaddr*)&caddr,&d);
		if(s>0){
		printf("server received: %s\n",buff);
		memset(buff,'\0',50);}
		sleep(1);
	}
}

void * writer(void *f)
{
	while(1)
	{
		scanf("%s",buffer);
		printf("scanned\n");
		buffer[strlen(buffer)]='\0';
		printf("scanned\n");
		inet_ntop(AF_INET,&(caddr.sin_addr),ad,20);
		printf("Client addr%s\n",ad );
		int p=(int)ntohs(caddr.sin_port);
		printf("%d\n",p);
		int g = sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&caddr, sizeof(saddr));
		printf("g=%d\n",g );
		sleep(1);
	}
}

int main()
{
	sfd= socket(AF_INET, SOCK_DGRAM, 0);
	/*caddr.sin_family=AF_INET;
	caddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	caddr.sin_port = htons(5051);
	*/saddr.sin_family=AF_INET;
	//saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_addr.s_addr = inet_addr("192.168.43.150");
	saddr.sin_port = htons(6060);
	int f = bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	printf("%d\n", f);
	inet_ntop(AF_INET,&(saddr.sin_addr),ad,20);
	printf("server addr%s\n",ad );
	inet_ntop(AF_INET,&(caddr.sin_addr),ad,20);
	printf("Client addr%s\n",ad );
	
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(r,NULL);
	pthread_join(w,NULL);
}
