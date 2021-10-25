#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<poll.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/shm.h>

int shmid;
int * ptr;

struct msg
{
	long type;
	char mtext[100];
};

int mid, turn = 1;

void handler(int signum)
{
	(*ptr)++;
	printf("live telecasts : %d\n",*(int*) ptr);
}


int main()
{
	signal(SIGUSR2,handler);
	shmid = shmget(101,sizeof(int),IPC_CREAT | 0666);
	ptr = (int *)shmat(shmid,(void*)0,0);
	*ptr=0;
	char buf[100], b[100];
	sprintf(b,"%d",getpid());
	strcat(buf,"#");
	strcat(buf,b);
	struct msg m1;
	m1.type=1;
	memset(m1.mtext,'\0',100);
	strcpy(m1.mtext,buf);
	printf("%s\n",buf);
	msgsnd(mid,&m1,sizeof(m1),IPC_NOWAIT);
	m1.type=2;
	msgsnd(mid,&m1,sizeof(m1),IPC_NOWAIT);
	
	mid = msgget(1234,0666|IPC_CREAT);
	mkfifo("./rep1",0666);
	mkfifo("./rep2",0666);
	mkfifo("./rep3",0666);
	mkfifo("./dw",0666);
	int r1,r2,r3;
	r1=open("./rep1",O_RDWR);
	r2=open("./rep2",O_RDWR);
	r3=open("./rep3",O_RDWR);
	int w = open("./dw",O_RDWR);
	printf("%d %d %d\n",r1,r2,r3);
	struct pollfd fd[3];
	fd[0].fd = (r1);
	fd[1].fd = (r2);
	fd[2].fd = (r3);
	for(int i=0;i<3;i++)
		fd[i].events=POLLIN;
	while(1)
	{
		int s = poll(fd,3,-1);
		printf("s=%d\n",s );
		if(s>0)
		{
			for(int i=0;i<3;i++)
			{
				if(fd[i].revents & POLLIN)
				{
					printf("i=%d\n",i );
					char buff[100];
					memset(buff,'\0',100);
					if(read(fd[i].fd,buff,100)>0)
					{
						printf("buff=%s\n",buff );
						if(buff[0]=='/' && buff[1]=='d')
						{
							char rp[8];
							if(i==0)
								strcpy(rp,"./rep1");
							else if(i==1)
								strcpy(rp,"./rep2");
							else
								strcpy(rp,"./rep3");
							fd[i].events=!(POLLIN);
							write(w,rp,strlen(rp));
						}
						else
						{
							struct msg m;
							m.type = turn;
							if(turn == 1)
								turn = 2;
							else
								turn = 1;
							strcpy(m.mtext,buff);
							msgsnd(mid,&m,sizeof(m),IPC_NOWAIT);
						}
					}
				}
			}
		}
	}
}
