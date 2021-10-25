#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/stat.h>
#include<fcntl.h>


int main()
{
	char buff[100];
	strcpy(buff,"hello");
	mkfifo("./rep3",0666);
	int r1=open("./rep3",O_RDWR);
	dup2(r1,1);
	while(1)
	{
		memset(buff,'\0',100);
	
		scanf("%s",buff);
		//printf("%s\n",buff);
		write(r1,buff,strlen(buff));
	}
}