#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>

int sfd,nsfd;
char buff[50],buffer[50];
pthread_t r,w;

void handler()
{
	int flags = fcntl(sfd,F_GETFL,0);
	fcntl(sfd,F_SETFL,flags|O_NONBLOCK);
	
	strcpy(buffer,"leaving");
	send(nsfd,buffer,strlen(buffer),0);
	while((nsfd=accept(sfd,NULL,NULL))!=-1)
	{
		printf("%d\n", nsfd);
		send(nsfd,buffer,strlen(buffer),0);
		nsfd=-1;
	}
	exit(0);
}

void * reader(void *f)
{
	while(1)
	{
		int s = recv(nsfd,buff,50,0);
		if(s>0){
			if(strcmp(buff,"leaving")==0)
			{
				pthread_cancel(w);
				pthread_exit(0);
			}
		printf("Server received: %s\n",buff);
		memset(buff,'\0',50);}
		sleep(1);
	}
}

void * writer(void *f)
{
	while(1)
	{
		scanf("%s",buffer);
		buffer[strlen(buffer)]='\0';
		send(nsfd,buffer,strlen(buffer),0);
		sleep(1);
	}
}

int main()
{
	signal(SIGINT,handler);
	sfd= socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(5050);
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd,SOMAXCONN);
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);

		strcpy(buff,"hello");
		write(nsfd,buff,strlen(buff));
		memset(buff,'\0',50);
		pthread_create(&w,NULL,&writer,NULL);
		pthread_create(&r,NULL,&reader,NULL);
		pthread_join(r,NULL);
		pthread_join(w,NULL);	
	}
}