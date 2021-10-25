#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

int sfd,port;
char buff[50],buffer[50];

void * reader(void *f)
{
	while(1)
	{
		int s = recv(sfd,buff,50,0);
		if(s>0){
		printf("Client received: %s\n",buff);
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
		send(sfd,buffer,strlen(buffer),0);
		sleep(1);
	}
}

int main()
{
	sfd= socket(AF_INET, SOCK_STREAM, 0);
	printf("%d\n", sfd);
	printf("Enter port\n");
	scanf("%d",&port);
	//char * addr;
	//addr = inet_ntop(AF_INET, , ,INET_ADDRSTRLEN);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(port);
	
	connect(sfd,(struct sockaddr*)&saddr, sizeof(saddr));

	//char buff[50];
	read(sfd,buff,20);
	printf("%s\n", buff);
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(r,NULL);
	pthread_join(w,NULL);	
}
