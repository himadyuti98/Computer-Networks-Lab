#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ipc.h>

int sfd[4];
struct sockaddr_in saddr;
struct pollfd fd[4];
int wfd;
char * service_name[]={"s1","s2","s3","s4"};
char * protocol[]={"TCP","TCP","UDP","TCP"};
int port[]={5511,5522,5533,5544};
int limit[]={2,1,SOMAXCONN,SOMAXCONN};
char * serv_pro_by[]={"process","thread","function","process"};

void * runner(void * f)
{
	while(1)
	{
		char buff[10];
		read(wfd,buff,10);
		int i=buff[0]-'0';
		printf("buff = %s i=%d\n",buff, i);
		limit[i]++;
	}
}

void * s2(void * f)
{
	int nsfd = *(int *)f;
	printf("nsfd=%d\n", nsfd);
	char buff[50];
	read(nsfd,buff,50);
	char b[10];
	sprintf(b,"%d",strlen(buff));
	write(nsfd,b,strlen(b));
	sleep(10);

}


int main()
{
	mkfifo("./wk",IPC_CREAT|0666);
	wfd = open("./wk",O_RDWR);
	pthread_t p;
	pthread_create(&p,NULL,runner,NULL);
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	for(int i=0;i<4;i++)
	{
		if(strcmp(protocol[i],"TCP")==0)
		{
			sfd[i]=socket(AF_INET, SOCK_STREAM, 0);
			//int flags = fcntl(sfd[i],F_GETFL,0);
			//fcntl(sfd[i],F_SETFL,flags|O_NONBLOCK);
		}
		else
			sfd[i]=socket(AF_INET, SOCK_DGRAM, 0);
		saddr.sin_port = htons(port[i]);
		bind(sfd[i],(struct sockaddr*)&saddr, sizeof(saddr));
		printf("listen=%d %d\n",listen(sfd[i],limit[i]),limit[i]);
		fd[i].fd = sfd[i];
		fd[i].events = POLLIN;
	}
	while(1)
	{
		int s = poll(fd,4,-1);
		//printf("s=%d\n",s );
		if(s>0)
		{
			for(int i=0;i<4;i++)
			{
				if(fd[i].revents & POLLIN)
				{
					printf("i=%d\n",i);
					if(strcmp(protocol[i],"TCP")==0 && limit[i]>0)
					{
						limit[i]--;
						/*if(limit[i]==0)
							fd[i].events = !(POLLIN);
						else
							fd[i].events = POLLIN;*/
						printf("limit=%d\n",limit[i]);
						int nsfd = accept(sfd[i],NULL,NULL);
						if(nsfd!=-1)
						{
							printf("nsfd=%d\n",nsfd);
							if(strcmp(serv_pro_by[i],"process")==0)
							{
								printf("one\n");
								int c = fork();
								if(c>0)
								{
									close(nsfd);
								}
								else
								{
									printf("one\n");
									char * a[]={"","","",NULL};
									char t[50],t1[10],t2[10];
									memset(t,'\0',50);
									memset(t1,'\0',10);
									memset(t2,'\0',10);
									//int old_stdin = dup(0);
									//int old_stdout = dup(1);
									sprintf(t1,"%d",nsfd);
									sprintf(t2,"%d",i);
									a[1]=t1;
									a[2]=t2;
									memset(t,'\0',50);
									//sprintf(t,"%d",old_stdout);
									//a[2]=t;
									//dup2(nsfd,0);
									//dup2(nsfd,1);
									strcpy(t,"./");
									strcat(t,service_name[i]);
									a[0]=t;
									printf("a[0]=%s\n",a[0]);
									execvp(a[0],a);
									/*char * a[]={"","","","",NULL};
									char t[50],t1[10],t2[10],t3[10];
									memset(t,'\0',50);
									memset(t1,'\0',10);
									memset(t2,'\0',10);
									memset(t3,'\0',10);
									int old_stdin = dup(0);
									int old_stdout = dup(1);
									sprintf(t1,"%d",old_stdin);
									sprintf(t2,"%d",old_stdout);
									sprintf(t3,"%d",i);
									a[1]=t1;
									a[2]=t2;
									a[3]=t3;
									strcpy(t,"./");
									strcat(t,service_name[i]);
									a[0]=t;
									dup2(nsfd,0);
									dup2(nsfd,1);
									execvp(a[0],a);*/
								}
							}
							
							else if(strcmp(serv_pro_by[i],"thread")==0)
							{
								printf("here\n");
								pthread_t h;
								pthread_create(&h,NULL,s2,&nsfd);
								pthread_join(h,NULL);
								limit[i]++;
								printf("here\n");
							}
							else
							{

							}
						}
						
					}
					else if(strcmp(protocol[i],"TCP")==0)
					{
						int nsfd = accept(sfd[i],NULL,NULL);
						char t[50];
						strcpy(t,"cant serve\n");
						write(nsfd,t,strlen(t));
						close(nsfd);
					}
					else
					{

					}
				}
			}
		}
	}
}