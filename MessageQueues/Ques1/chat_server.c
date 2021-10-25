#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include<string.h>
#include<stdlib.h>

int m1,m2;

struct msg
{
	long mtype;
	char mtext[100];
};

int present(int c[], int n, int p)
{
	for(int i=0;i<n;i++)
		if(p==c[i])
			return 1;
	return 0;
}
int main()
{
	m1=msgget(3838,0666|IPC_CREAT);
	//m2=msgget(83,0666|IPC_CREAT);
	printf("m1=%dm2=%d\n", m1,m2);
	struct msg mt[50];
	int cl[50];
	int c=0;
	while(1)
	{
		sleep(1);
		char buff[50];
		struct msg k;
		int s = msgrcv(m1,&k,sizeof(k),1,0);
		printf("s=%d\n",s );
		if(s!=-1)
		{
			strcpy(buff,k.mtext);
			printf("%s\n", buff);
			char a[5];
			int i;
			for(i=0;buff[i]!='$';i++)
				a[i]=buff[i];
			a[i]='\0';
			int p=atoi(a);
			if(!present(cl,c,p))
			{
				cl[c]=p;
				struct msg m;
				m.mtype=p;
				for(int i=0;i<strlen(buff);i++)
					m.mtext[i]=buff[i];
				mt[c++]=m;
			}
			for(int j=0;j<c;j++)
			{
				for(int i=0;i<strlen(buff);i++)
					mt[j].mtext[i]=buff[i];
				msgsnd(m1,&mt[j],sizeof(mt[j]),0);
				printf("%ld\n",mt[j].mtype );
				//msgsnd(m2,buff,strlen(buff),0);
			}
		}
	}
	return 0;
}
