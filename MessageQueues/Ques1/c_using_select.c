#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include<string.h>

int m1,m2;
struct msg
{
	long mtype;
	char mtext[100];
	int f;
};

void * reader(void * f)
{
	while(1)
	{
		struct msg m;
		memset(m.mtext,'\0',100);
		msgrcv(m1,&m,sizeof(m),getpid(),0);
		printf("%s\n",m.mtext);

	}
}

void * writer(void * f)
{
	while(1)
	{
		char b[20],buff[50];
		sprintf(buff,"*%d$",getpid());
		scanf("%s",b);
		strcat(buff,b);
		struct msg m;
		//m.mtype=0;
		m.f=1;
		for(int i=0;i<strlen(buff);i++)
			m.mtext[i]=buff[i];
		//printf("%s\n",buff );
		//msgsnd(m1,&m,sizeof(m),0);
		msgsnd(m1,buff,strlen(buff),0);
	}
}

int main(int argc, char const *argv[])
{
	m1=msgget(3838,0666|IPC_CREAT);
	
	/*pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(w,NULL);
	pthread_join(r,NULL);*/

	int m=m1;

	fd_set r,w;
	FD_ZERO(&r);
	FD_SET(0,&r);
	struct timeval t;
	t.tv_sec=1;
	t.tv_usec=0;
	while(1)
	{
		printf("**\n");
		int s = select(1,&r,NULL,NULL,NULL);
		printf("s=%d\n",s);
		if(s>0)
		{
			printf("s=%d\n",s);
			if(FD_ISSET(0,&r))
			{
			char b[20],buff[50];
			sprintf(buff,"%d$",getpid());
			scanf("%s",b);
			strcat(buff,b);
			struct msg m;
			m.mtype=1;
			for(int i=0;i<strlen(buff);i++)
				m.mtext[i]=buff[i];
			//printf("%s\n",buff );
			//msgsnd(m1,buff,strlen(buff),0);
			msgsnd(m1,&m,sizeof(m),0);
			
			}
		}
		struct msg m;
		memset(m.mtext,'\0',100);
		//printf("##\n");
				
		s = msgrcv(m1,&m,sizeof(m),getpid(),0);
		

		//printf("##\n");
		if(s!=-1)
		{
			printf("%s\n",m.mtext);
		}
		FD_ZERO(&r);
		FD_SET(0,&r);	
		
	}
	return 0;
}
