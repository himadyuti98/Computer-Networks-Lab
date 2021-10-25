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
#include<pthread.h>

struct sockaddr_in server,client,client2;
char port[20];

void * reader(void *f)
{
	int nsfd = *(int* )f;
	char buff[50];
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
	int nsfd = *(int* )f;
	char buffer[50];
	while(1)
	{
		scanf("%s",buffer);
		buffer[strlen(buffer)]='\0';
		send(nsfd,buffer,strlen(buffer),0);
		sleep(1);
	}
}

void * raw(void * f)
{
	int rsfd = *(int*)f;
	socklen_t clilen=sizeof(client);
	char buf[50];
	while(1)
	{
		client2 = client;
		bzero(&client,sizeof(client));
		int s = recvfrom(rsfd,buf,50,0,(struct sockaddr*)&client,(socklen_t*)&clilen);
		//perror("recv");
		struct iphdr *ip;
		ip=(struct iphdr*)buf;
		int x = (ip->ihl)*4;
		printf("%s\n",buf+x);
		if(s>0)// && strcmp(buf+x,"info")==0)// && client2.sin_addr.s_addr!=client.sin_addr.s_addr)
		{
			char ad[20];
			inet_ntop(AF_INET,&(client.sin_addr),ad,20);
			printf("Client addr%s\n",ad );
			int p=(int)ntohs(client.sin_port);
			printf("%d\n",p);
			sendto(rsfd,port,strlen(port)+1,0,(struct sockaddr*)&client,sizeof(client));
			bzero(&client,sizeof(client));
			perror("send");
			sleep(2);
			s=0;
			//pthread_t p1;
			//pthread_create(&p1,NULL,raw,&rsfd);
			//pthread_join(p1,NULL);
			//break;
			
		}
	}
}


int main(int argc, char* argv[])
{
	strcpy(port,argv[2]);
	//strcat(port,"\n");
	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	if(rsfd==-1) perror("socket");
	int flags = fcntl(rsfd,F_GETFL,0);
	fcntl(rsfd,F_SETFL,flags|!(O_NONBLOCK));
	pthread_t rw ;
	pthread_create(&rw,NULL,raw,&rsfd);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t servlen=sizeof(server);
	bind(rsfd,(struct sockaddr*)&server,sizeof(server));

	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
		perror("connection oriented socket");
	server.sin_port = atoi(argv[2]);
	bind(sfd, (struct sockaddr*)&server, sizeof(server));
	listen(sfd,SOMAXCONN);
	while(1)
	{
		int nsfd = accept(sfd,NULL,NULL);
		int c = fork();
		if(c==0)
		{
			close(sfd);
			char buff[50];
			strcpy(buff,"hello");
			write(nsfd,buff,strlen(buff));
			memset(buff,'\0',50);
			pthread_t r,w;
			pthread_create(&w,NULL,&writer,&nsfd);
			pthread_create(&r,NULL,&reader,&nsfd);
			pthread_join(r,NULL);
			pthread_join(w,NULL);
			close(nsfd);
			exit(0);
		}
		else
		{
			close(nsfd);
		}
	}

}