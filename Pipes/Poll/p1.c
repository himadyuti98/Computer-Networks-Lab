#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<poll.h>

int main()
{
	FILE* fd2,*fd3,*fd4,*fd5,*fd6;
	fd2=popen("./p2","r");
	fd3=popen("./p3","r");
	fd4=popen("./p4","r");
	fd5=popen("./p5","r");
	fd6=popen("./p6","w");
	printf("5**************\n");
	struct pollfd pfd[4];
	pfd[0].fd=fileno(fd2);
	pfd[1].fd=fileno(fd3);
	pfd[2].fd=fileno(fd4);
	pfd[3].fd=fileno(fd5);
	for(int i=0;i<4;i++)
		pfd[i].events=POLLOUT;
	char g[20];
	printf("8**************\n");
	while(1)
	{
		//printf("**************");
		sleep(1);
		int s = poll(pfd,4,50);
		printf("s=%d\n",s);
		if(s>0)
		{
			for(int i=0;i<4;i++)
			{
				if(pfd[i].revents)// & POLLIN)
				{
					printf("**************");
					//f=1;
					/*int stdinback=dup(0);
					dup2(pfd[i].fd,0);
					scanf("%s",g);
					printf("%s\n",g);
					dup2(stdinback,0);*/
					int fd=fileno(fd6);
					dup2(fd,1);
					scanf("%s",g);
					printf("%s\n",g);
				}
				pfd[i].events=POLLIN;
			}
		}
	}
	return 0;
}
