#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<poll.h>

int main()
{
	struct pollfd pfd[4];
	char buffer[16];
	int timeout=50;
	int ret;
	FILE *fp;
	fp=popen("./poll2","r");
	pfd[0].fd=fileno(fp);
	pfd[1].fd=fileno(popen("./poll3","r"));
	pfd[2].fd=fileno(popen("./poll4","r"));
	pfd[3].fd=fileno(popen("./poll5","r"));
	pfd[0].events=POLLOUT;
	pfd[1].events=POLLOUT;
	pfd[2].events=POLLOUT;
	pfd[3].events=POLLOUT;

while(1)
{
	ret=poll(pfd,4,timeout);
	printf("%d\n",ret);
	if(ret>0)
	{
		int i;
		for(i=0;i<4;i++)
		{
			if(pfd[i].revents)
			{
                       		printf("entered\n");
				if(read(pfd[i].fd,buffer,17)>0)	
					printf("in poll1 %s\n",buffer);
                             	FILE *fp1;
                         	fp1=popen("./poll6","w");
                        	int fd=fileno(fp1);
                         	dup2(fd,1);
				printf("%s",buffer);
			}
		}
	}
	else if(ret==0)
	{
		FILE *fp1;
		fp1=popen("./poll6","w");
		int fd1=fileno(fp1);
		dup2(fd1,1);
		printf("Info from poll 1");
	}
//sleep(2);
}
	return 0;
}
