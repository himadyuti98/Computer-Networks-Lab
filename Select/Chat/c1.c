#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

char buff1[100], buff2[100];
int fspcl,fd;

int main()
{
	mkfifo("./cl1",0666);
	fd=open("./cl1",O_RDWR);
	mkfifo("./spcl",0666);
	fspcl=open("./spcl",O_RDWR);
	
	/*pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(r,NULL);
	pthread_join(w,NULL);	*/

	int m=fd;

	fd_set r,w;
	FD_ZERO(&r);
	FD_ZERO(&w);
	FD_SET(fd,&r);
	FD_SET(0,&r);
	while(1)
	{
		int s = select(m+1,&r,NULL,NULL,NULL);
		if(s>0)
		{
			if(FD_ISSET(fd,&r))
			{
				FD_CLR(fd,&r);
				read(fd,buff1,100);
				printf("Client1 received: %s\n",buff1);
				memset(buff1,'\0',100);
			}
			if(FD_ISSET(0,&r))
			{
				FD_CLR(0,&r);
				scanf("%s",buff2);
				char s[20]="./cl1%";
				strcat(s,buff2);
				write(fspcl,s,sizeof(s));
			}
		}
		FD_ZERO(&r);
		FD_SET(fd,&r);
		FD_SET(0,&r);
	}
	
		
	return 0;
}
