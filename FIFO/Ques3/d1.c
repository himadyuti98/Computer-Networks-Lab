#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

int main()
{
	mkfifo("./ds1",0666);
	int fd=open("./ds1",O_RDWR);
	while(1)
	{
		dprintf(fd,"I'm D1\n");
		sleep(1);
	}
}
