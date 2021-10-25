#include<stdio.h>
#include<sys/socket.h>
#include<#include<stdio.h>
#include<sys/socket.h>
#include<выVe <sys/un.h>
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

struct sockaddr_in server, server2;

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t servlen=sizeof(server);
	server.sin_port = atoi(argv[2]);
	bind(sfd, (struct sockaddr*)&server, sizeof(server));
	listen(sfd,SOMAXCONN);
	while(1)
	{
		int nsfd = accept(sfd,NULL,NULL);
		int c = fork();
		if(c>0)
		{
			close(nsfd);
		}
		else
		{
			int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
			if(rsfd==-1) 
				perror("socket");
			char buff[50];
			while(1){
				memset(buff,'\0',50);
			recv(nsfd,buff,50,0);
			//buff[strlen(buff)-1]='\0';
			printf("%s\n", buff);
			int i;
			char ip[50];
			char file[50];
			for (i = 0; i < strlen(buff); ++i)
			{
				if(buff[i]=='$')
					break;
				ip[i]=buff[i];
			}
			ip[i]='\0';
			i++;
			int j;
			for(j=0;i<strlen(buff);i++,j++)
			{
				file[j]=buff[i];
			}
			file[j]='\0';
			server2.sin_family=AF_INET;
			server2.sin_addr.s_addr=inet_addr(ip);
			printf("%s %s\n", ip,file);
			int f = connect(rsfd,(struct sockaddr*)&server2, sizeof(server2));
			if(f<0)
				perror("connect");
			else
				printf("connected\n");
			int fd = open(file,O_RDWR);
			perror("opem");
			printf("%d\n", fd);
			char buf[50];
		//	while()
			{
				printf("reading\n");
				read(fd,buf,50);
				printf("%s\n", buf);
				write(rsfd,buf,strlen(buf));
			}
			close(fd);
			close(rsfd);}
			close(nsfd);
			
		}
	}
	return 0;
}