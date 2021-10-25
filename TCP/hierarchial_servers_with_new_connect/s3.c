#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

int sfd,nsfd;
char buff[50],buffer[50];

void * reader(void *f)
{
	while(1)
	{
		int s = recv(nsfd,buff,50,0);
		if(s>0){
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

int main(int argc, char* argv[])
{
	sfd= socket(AF_INET, SOCK_STREAM, 0);
	//char * addr;
	//addr = inet_ntop(AF_INET, , ,INET_ADDRSTRLEN);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(5003);
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd,SOMAXCONN);

	//while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		int c = fork();
		if(c==0)
		{
			close(sfd);
			strcpy(buff,"hello");
			write(nsfd,buff,strlen(buff));
			memset(buff,'\0',50);
			pthread_t r,w;
			pthread_create(&w,NULL,&writer,NULL);
			pthread_create(&r,NULL,&reader,NULL);
			pthread_join(r,NULL);
			pthread_join(w,NULL);
			close(nsfd);
		}
		else
		{
			close(nsfd);
		}
	}

	

	//char buff[50];
	/*strcpy(buff,"hello");
	write(nsfd,buff,strlen(buff));
	memset(buff,'\0',50);
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(r,NULL);
	pthread_join(w,NULL);	*/
}