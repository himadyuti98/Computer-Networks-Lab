#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<poll.h>
#include<string.h>
#include<sys/types.h>

int main()
{
	FILE *f2=popen("./p2","r");
	FILE *f3=popen("./p3","r");
	FILE *f4=popen("./p4","r");
	FILE *f5=popen("./p5","r");
	FILE *f6=popen("./p6","w");
	//struct pollfd f[4];
	int f[4];
	f[0]=fileno(f2);
	f[1]=fileno(f3);
	f[2]=fileno(f4);
	f[3]=fileno(f5);
	int m = -1;
	fd_set fr;
	FD_ZERO(&fr);
	for(int i=0;i<4;i++)
	{
		FD_SET(f[i], &fr);
		if(m<f[i])
			m=f[i];
	}
	while(1)
	{
		int s = select(m+1,&fr,NULL,NULL,NULL);
		printf("s=%d\n",s);
		if(s>0)
		{
			for(int i=0;i<4;i++)
			{
				if(FD_ISSET(f[i],&fr))
				{
					//FD_CLR(f[i],&fr);
					char buff[20];
					dup2(f[i],0);
					dup2(fileno(f6),1);
					scanf("%s",buff);
					//printf("%s\n",buff);
					//fflush(stdout);
					//fflush(stdin);
					//read(f[i].fd,buff,20);
					//write(fileno(f6),buff,strlen(buff));
					printf("%s\n",buff);
					fflush(stdout);
					fflush(stdin);
					sleep(1);
				}
			}
		}
		
	}
}
