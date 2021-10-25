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
	m1=msgget(838,0666|IPC_CREAT);
	//m2=msgget(83,0666|IPC_CREAT);
	printf("m1=%dm2=%d\n", m1,m2);
	struct msg mt[50];
	int cl[5][50];
	int c[5]={0},d=0;
	while(1)
	{
		char buff[50];
		struct msg k;
		int s = msgrcv(m1,&k,sizeof(k),1,0);
		printf("s=%d\n",s );
		if(s!=-1)
		{
			strcpy(buff,k.mtext);
			printf("%s\n", buff);
			char a[5];
			int k=buff[0]-'0';
			int i;
			for(i=2;buff[i]!='$';i++)
				a[i-2]=buff[i];
			a[i-2]='\0';
			int p=atoi(a);
			printf("p=%d\n",p);
			if(!present(cl[k],c[k],p))
			{
				cl[k][c[k]++]=p;
				struct msg m;
				m.mtype=p;
				for(int i=0;i<strlen(buff);i++)
					m.mtext[i]=buff[i];
				mt[d++]=m;
			}
			for(int j=0;j<c[k];j++)
			{
				for(int x=0;x<d;x++)
				{
					if(mt[x].mtype==cl[k][j])
					{
						memset(mt[x].mtext,'\0',100);
						for(int i=0;i<strlen(buff);i++)
							mt[x].mtext[i]=buff[i];
						printf("pid=%ld,msg=%s\n",mt[x].mtype,mt[x].mtext);
						msgsnd(m1,&mt[x],sizeof(mt[x]),0);
					}
				}
				
				
				//printf("%ld\n",mt[j].mtype );
				//msgsnd(m2,buff,strlen(buff),0);
			}
		}
	}
	return 0;
}
