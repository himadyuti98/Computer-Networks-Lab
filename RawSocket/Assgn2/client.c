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

struct sockaddr_in server,client;

void * service_reader(void * f)
{
	int sfd = *(int *)f;
	char buff[50];
	while(1)
	{
		memset(buff,'\0',50);
		int s = read(sfd,buff,50);
		if(s>0)
		{
			printf("%s\n", buff);
		}
	}
}


void * keyboard(void * f)
{
	int rsfd = *(int*)f;
	char buff[20];
	int sfd[20];
	int n=0;
	while(1)
	{
		scanf("%s",buff);
		if(strcmp(buff,"info")==0)
		{
			sendto(rsfd,buff,strlen(buff)+1,MSG_EOR,(struct sockaddr*)&server,sizeof(server));
			perror("sent");
		}
		else if(buff[0]=='p')
		{
			char buf[20];
			for(int i=0;i<strlen(buff)-1;i++)
				buf[i]=buff[i+1];
			int port = atoi(buf);
			sfd[n] = socket(AF_INET,SOCK_STREAM,0);
			server.sin_port = port;
			connect(sfd[n],(struct sockaddr*)&server, sizeof(server));
			perror("connect");
			pthread_t p;
			pthread_create(&p, NULL, service_reader, &sfd[n]);
			n++;
		//	pthread_t p1;
		//	pthread_create(&p1, NULL, service_writer, &sfd);
		}
		else
		{
			int p = buff[0]-'0';
			char buf[20];
			int i;
			for(i=0;i<strlen(buff)-1;i++)
				buf[i]=buff[i+1];
			buf[i]='\0';
			write(sfd[p],buf,strlen(buff));
		}
	}
}

void * raw(void * f)
{
	int rsfd = *(int*)f;
	char buf[20];
	socklen_t servlen=sizeof(server);
	//while(1)
	{
		int s;
		xyz:
		 s = recvfrom(rsfd,buf,50,0,(struct sockaddr*)&server,(socklen_t*)&servlen);
		if(s>0)
		{
			struct iphdr *ip;
			ip=(struct iphdr*)buf;
			int x = (ip->ihl)*4;
   		    printf("%s\n",buf+x );
   		    s=0;
   		    goto xyz;
		}
	}
}

int main(int argc, char * argv[])
{
	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	if(rsfd==-1) perror("socket");
	int optval=1;
	//setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));
	int flags = fcntl(rsfd,F_GETFL,0);
	fcntl(rsfd,F_SETFL,flags|!(O_NONBLOCK));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t servlen=sizeof(server);
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.2");
	bind(rsfd,(struct sockaddr*)&client,sizeof(client));
	pthread_t key,r;
	pthread_create(&key,NULL,keyboard,&rsfd);
	pthread_create(&r,NULL,raw,&rsfd);
	pthread_join(key,NULL);
}