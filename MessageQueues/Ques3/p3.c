#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<fcntl.h>

int m1;

static int *x,*y,*z; 

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
		msgrcv(m1,&m,sizeof(m),2,0);
		printf("%s\n",m.mtext);

	}
}

void * writer(void * f)
{
	while(1)
	{
		char b[20];
		scanf("%s",b);
		struct msg m;
		m.mtype=1;
		memset(m.mtext,'\0',100);
		for(int i=0;i<strlen(b);i++)
			m.mtext[i]=b[i];
		//printf("%s\n",buff );
		msgsnd(m1,&m,sizeof(m),0);
	}
}

int main(int argc, char const *argv[])
{
	m1=msgget(3838,0666|IPC_CREAT);
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(w,NULL);
	pthread_join(r,NULL);
	

	return 0;
}