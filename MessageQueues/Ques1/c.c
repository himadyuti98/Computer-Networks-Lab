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

int main(int argc, char const *argv[])
{
	m1=msgget(383,0666|IPC_CREAT);
	m2=msgget(83,0666|IPC_CREAT);
	printf("m1=%dm2=%d\n", m1,m2);
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(w,NULL);
	pthread_join(r,NULL);
	return 0;
}
