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
	mkfifo("./screen",0666);
	int r1=open("./screen",O_RDWR);
	while(1)
	{
		memset(buff,'\0',100);
		read(r1,buff,100);
		printf("%s\n",buff);
		
	}
}