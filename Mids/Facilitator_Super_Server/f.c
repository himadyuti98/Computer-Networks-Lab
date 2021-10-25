#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<stdlib.h>
#include<signal.h>

char buff[30];
struct sockaddr_in saddr,saddr1[50],caddr[50],caddr1[20];
int sfd[50],wsfd;
int n=0,nc=0;
struct pollfd fd[50];
char proc[50][10];
int proid[50];
int port[50];

void handler(int signum)
{
	int c = fork();
	if(c==0)
	{
		printf("child %d %s\n",c,proc[n]);
		char * a[] = {"",NULL};
		printf("***%d\n",port[n]);
		char t[5];
		sprintf(t,"%d",port[n]);
		a[0]=t;
		printf("***\n");
		printf("%s\n",proc[n] );
		execvp(proc[n],a);

	}
	else
	{
		printf("parent %d\n",c);
		proid[n++]=c;
	}
}

void * runner(void *f)
{
	while(1)
	{
		printf("i=0\n");
		//struct sockaddr_in c;
		char b[50];
		memset(b,'\0',50);
		socklen_t l = (socklen_t)sizeof(caddr[nc]);
		int s = recvfrom(wsfd,b,50,0,(struct sockaddr*)&caddr[nc],&l);
		if(s>0)
		{
		char ad[20];
		inet_ntop(AF_INET,&(caddr[nc].sin_addr),ad,20);
		int p=(int)ntohs(caddr[nc].sin_port);
		printf("Client addr%s %d n=%d\n",ad,p,n );
		char buff[100];
		memset(buff,'\0',100);
		for(int j=0;j<n;j++)
		{
			memset(b,'\0',50);
			sprintf(b,"%d ",port[j]);
			strcat(b,proc[j]);
			strcat(buff,b);
			strcat(buff,"\n");
		}
		sendto(wsfd,buff,strlen(buff),0,(struct sockaddr*)&caddr[nc],sizeof(caddr[nc]));
		nc++;
		for(int i=0;i<nc;i++)
			{
				char ad[20];
		inet_ntop(AF_INET,&(caddr[i].sin_addr),ad,20);
		int p=(int)ntohs(caddr[i].sin_port);
		printf("Client addr%s %d\n",ad,p);
		
			}
		}
	}
}

void * reader(void *f)
{
	while(1)
	{
		int s = scanf("%s",buff);
		if(s>0)
		{
			int i=0;
			char a[5];
			while(buff[i]!='$')
			{	
				//printf("%c\n",buff[i] );
				a[i]=buff[i];
				i++;
			}
			printf("%s\n",a );
			a[i]='\0';
			printf("%s\n",a );
			port[n]=atoi(a);
			char name[20];
			int j=0;i++;
			while(buff[i]!='\0')
			{
				name[j++]=buff[i++];
			}
			name[j]='\0';
			printf("%d %s\n",port[n],name );
			strcpy(proc[n],name);
			sfd[n]=socket(AF_INET,SOCK_DGRAM,0);
			saddr1[n].sin_family=AF_INET;
			saddr1[n].sin_addr.s_addr = inet_addr("127.0.0.1");
			saddr1[n].sin_port = htons(port[n]);
			if(setsockopt(sfd[n],SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT, &(int){1},sizeof(int))<0)
			{
				perror("failed in f\n");
			}
			int f1 = bind(sfd[n], (struct sockaddr*)&saddr1[n], sizeof(saddr1[n]));
			printf("hoho\n");			
			while(f1==-1)
			{
				f1 = bind(sfd[n], (struct sockaddr*)&saddr1[n], sizeof(saddr1[n]));
			}
			printf("ready\n");
			listen(sfd[n],10);
			fd[n].fd=sfd[n];
			fd[n].events=POLLIN;
			kill(getpid(),SIGUSR1);
			char b[50];
			memset(buff,'\0',100);
				for(int j=0;j<=n;j++)
				{
					memset(b,'\0',50);
					sprintf(b,"%d ",port[j]);
					strcat(b,proc[j]);
					strcat(buff,b);
					strcat(buff,"\n");
				}
				printf("%s\n", buff);
			for(int i=0;i<nc;i++)
			{
				char ad[20];
		inet_ntop(AF_INET,&(caddr[i].sin_addr),ad,20);
		int p=(int)ntohs(caddr[i].sin_port);
		printf("Client addr%s %d\n",ad,p);
		printf("sent\n");
				sendto(wsfd,buff,strlen(buff),0,(struct sockaddr*)&caddr[i],sizeof(caddr[i]));
			}
		}
		sleep(1);
	}
}

int main()
{
	signal(SIGUSR1,handler);
	wsfd= socket(AF_INET, SOCK_DGRAM, 0);
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(5050);
	//fd[0].fd=wsfd;
	//fd[0].events=POLLIN;
	int f = bind(wsfd, (struct sockaddr*)&saddr, sizeof(saddr));
	printf("%d\n", f);
	if(f==0)
	{
		pthread_t r,w;
		pthread_create(&r,NULL,&reader,NULL);
		pthread_create(&w,NULL,&runner,NULL);
		while(1)
		{
			int s = poll(fd,n,-1);
			printf("s=%d n=%d\n", s,n);
			if(s>0)
			{
				for(int i=0;i<n;i++)
				{
					if(fd[i].revents & POLLIN)
					{
						
						printf("i=%d\n", i);
						kill(proid[i],SIGUSR2);
						//fd[i].fd*=-1;
						fd[i].events=!(POLLIN);
						printf("signal sent %d\n",proid[i]);
						break;
						//sleep(2);
					}
				}
			}
			else if(s==-1)
				perror("error");
		}
	}
}
