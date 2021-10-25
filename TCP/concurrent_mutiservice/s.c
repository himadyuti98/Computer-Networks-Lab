#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<stdlib.h>


char buff[50],buffer[50];
int nsfd;

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
		printf("sc done\n");
		buffer[strlen(buffer)]='\0';
		send(nsfd,buffer,strlen(buffer),0);
		sleep(1);
	}
}


int main(int argc, char * argv[])
{
	char buf[20],b[20];
	
	nsfd=dup(fileno(stderr));
	printf("%d\n", nsfd);
	printf("flushing\n");
	fflush(stdin);
	fflush(stdout);
	fflush(stderr);
	//dup2(nsfd,1);
	printf("flushed\n");	
	printf("fd=%d\n", nsfd);
	/*printf("%d %s %s\n",argc, argv[0],argv[1]);
	nsfd=atoi(argv[0]);
	printf("%d\n", nsfd);
	strcpy(buffer,"honey");
	printf("%s\n",buffer );
	send(nsfd,buffer,strlen(buffer),0);*/
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(r,NULL);
	pthread_join(w,NULL);
	/*while(1){
	scanf("%s",buf);
	sprintf(b,"in s %s\n",buf);
	write(nsfd,b,strlen(b));
	}*/
}
