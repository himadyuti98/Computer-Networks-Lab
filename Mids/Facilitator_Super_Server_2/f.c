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
struct sockaddr_in saddr,saddr1[50];
int sfd[50],wsfd;
int n=0;
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
	
	int f = bind(wsfd, (struct sockaddr*)&saddr, sizeof(saddr));
	printf("%d\n", f);
	if(f==0)
	{
		pthread_t r;
		pthread_create(&r,NULL,&reader,NULL);
		char buff[50];
		while(1)
		{
			memset(buff,'\0',50);
			int s = recvfrom(wsfd,buff,50,0,NULL,NULL);
			if(s>0 && buff[0]>='0' && buff[0]<='9'){
				int p = atoi(buff);
				for(int i=0;i<n;i++)
				{
					if(p==port[i])
					{
						kill(proid[i],SIGUSR2);
						break;
					}
				}
			}
		}
	}
}
