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

char c[100];

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
		char b[20],buff[50],c2[50];
		sprintf(c2,"%s",c);
		sprintf(buff,"%d$",getpid());
		scanf("%s",b);
		strcat(buff,b);
		strcat(c2,buff);
		struct msg m;
		m.mtype=1;
		strcpy(m.mtext,c2);
		msgsnd(m1,&m,sizeof(m),0);
	}
}

int main(int argc, char const *argv[])
{
	m1=msgget(838,0666|IPC_CREAT);
	printf("m1=%dm2=%d\n", m1,m2);
	printf("Which group?\n");
	scanf("%s",c);
	strcat(c,"$");
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(w,NULL);
	pthread_join(r,NULL);
	return 0;
}
