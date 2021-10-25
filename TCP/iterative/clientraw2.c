#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>

int sfd,nsfd,rsfd;
char buff[65536],buffer[65536];
struct sockaddr_in client;
socklen_t clilen=sizeof(client);
	

void * reader(void *f)
{
	while(1)
	{
		int s = recv(sfd,buff,50,0);
		if(s>0){
		printf("Client received: %s\n",buff);
		memset(buff,'\0',65536);}
		sleep(1);
	}
}
void * readerraw(void *f)
{
	while(1)
	{
		int s = recvfrom(rsfd,buff,65536,0,NULL,NULL);
	if(s>0){
	perror("recv in raw");
	 struct iphdr *ip;
  	ip=(struct iphdr*)buff;
  	int x = (ip->ihl)*4;
	struct tcphdr *tcph = (struct tcphdr *)(buff+4*ip->ihl);
	
	  printf("%s\n",buff+ x + 8);}//tcph->doff*4);}
	}
}

void * writer(void *f)
{
	while(1)
	{
		scanf("%s",buffer);
		buffer[strlen(buffer)]='\0';
		send(sfd,buffer,strlen(buffer),0);
		sleep(1);
	}
}

int main()
{
	printf("%d\n", sfd);
	//char * addr;
	//addr = inet_ntop(AF_INET, , ,INET_ADDRSTRLEN);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	//saddr.sin_addr.s_addr = inet_addr("172.30.34.214");
	//saddr.sin_addr.s_addr = inet_addr("192.168.43.124");
	saddr.sin_addr.s_addr = inet_addr("127.0.0.2");
	saddr.sin_port = htons(9090);
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.2");
	bind(sfd,(struct sockaddr*)&client,clilen);
	rsfd=socket(AF_INET,SOCK_RAW,17);
	if(rsfd==-1) perror("socket");
		char buf[50];
	bind(rsfd,(struct sockaddr*)&client,clilen);
	pthread_t r,w,raw;
	//pthread_create(&w,NULL,&writer,NULL);
	//pthread_create(&r,NULL,&reader,NULL);
	pthread_create(&raw,NULL,&readerraw,NULL);
	
	pthread_join(raw,NULL);
//	pthread_join(w,NULL);	
}
