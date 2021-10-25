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

int sfd[20];
int n;
char buff[50],buffer[50];

void * reader(void *f)
{
	int nsfd = *(int* )f;
	while(1)
	{
		int s = recv(nsfd,buff,50,0);
		if(s>0){
		printf("Client received: %s\n",buff);
		memset(buff,'\0',50);}
		sleep(1);
	}
}

void * writer(void *f)
{
	int nsfd = *(int* )f;
	while(1)
	{
		scanf("%s",buffer);
		buffer[strlen(buffer)]='\0';
		send(nsfd,buffer,strlen(buffer),0);
		sleep(1);
	}
}


int main(int argc, char* argv[])
{
	n = argc-1;
	pthread_t r[n],w[n];
	for(int i=1;i<argc;i++)
	{
		sfd[i-1]= socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in saddr;
		saddr.sin_family=AF_INET;
		saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		saddr.sin_port = htons(atoi(argv[i]));
		
		connect(sfd[i-1],(struct sockaddr*)&saddr, sizeof(saddr));
		perror("connect");

	}
	
	for(int i=0;i<n;i++)
	{
		pthread_create(&w[i],NULL,&writer,&sfd[i]);
		pthread_create(&r[i],NULL,&reader,&sfd[i]);	
	}
	for(int i=0;i<n;i++)
	{
		pthread_join(r[i],NULL);
		pthread_join(w[i],NULL);
	}
	
}