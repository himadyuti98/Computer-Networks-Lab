#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<fcntl.h>
#include<sys/stat.h>


struct msg
{
	long type;
	char mtext[100];
};

int mid, turn, t, flag = 0, pid , epid;

void handler(int signum)
{
	flag = 1-flag;
}

int main(int argc, char * argv[])
{
	mid = msgget(1234,0666|IPC_CREAT);
	signal(SIGUSR1,handler);
	turn = atoi(argv[1]);
	if(turn == 1)
		t=2;
	else
		t=1;
	char buff[100], b[100];
	memset(b,'\0',100);
	memset(buff,'\0',100);
	sprintf(b,"%d",getpid());
	strcat(buff,"$");
	strcat(buff,b);
	struct msg m;
	m.type=t;
	memset(m.mtext,'\0',100);
	strcpy(m.mtext,buff);
	printf("%s\n",buff);
	msgsnd(mid,&m,sizeof(m),IPC_NOWAIT);
	fd_set r;
	FD_ZERO(&r);
	FD_SET(0,&r);
	mkfifo("./screen",0666|IPC_CREAT);
	int scr = open("./screen",O_RDWR);

	while(1)
	{
		while(flag==1);
		int s;// = select(1,&r,NULL,NULL,NULL);
		/*if(s>0)
		{
			if(FD_ISSET(0,&r))
			{
					//see count
			}
		}*/
		memset(m.mtext,'\0',100);
				
		s = msgrcv(mid,&m,sizeof(m),turn,IPC_NOWAIT|MSG_NOERROR);
		
		if(s!=-1)
		{
			//printf("%s\n",m.mtext);
			if(m.mtext[0]>='0' && m.mtext[0]<='9')
			{
				kill(pid,SIGUSR1);
				kill(epid,SIGUSR2);
				int port = atoi(m.mtext);
				struct sockaddr_in saddr;
				saddr.sin_family=AF_INET;
				saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
				saddr.sin_port = htons(port);
				int sfd = socket(AF_INET, SOCK_STREAM, 0);
				connect(sfd,(struct sockaddr*)&saddr, sizeof(saddr));
				char buff[50];
				while(1)
				{
					memset(buff,'\0',50);
					if(read(sfd,buff,50)>0){
					if(buff[0]=='x')
					{
						kill(pid,SIGUSR1);
						break;
					}
					char buf[140];
					sprintf(buf,"live telecast:%s",buff);
					write(scr,buf,strlen(buf));}
				}
			}
			else if(m.mtext[0]=='$')
			{
				printf("%s\n", m.mtext);
				char s[20];
				for(int i=1;i<strlen(m.mtext);i++)
				{
					s[i-1]=m.mtext[i];
				}
				s[strlen(m.mtext)-1]='\0';
				pid = atoi(s);
			}
			else if(m.mtext[0]=='#')
			{
				printf("%s\n", m.mtext);
				char s[20];
				for(int i=1;i<strlen(m.mtext);i++)
				{
					s[i-1]=m.mtext[i];
				}
				s[strlen(m.mtext)-1]='\0';
				epid = atoi(s);
			}
			else
			{
				char buf[140];
				sprintf(buf,"from news reader %d:%s",turn,m.mtext);
				write(scr,buf,strlen(buf));
			}
		}
		FD_ZERO(&r);
		FD_SET(0,&r);	
		
	}

}