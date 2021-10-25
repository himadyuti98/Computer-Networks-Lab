#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

int main()
{
	mkfifo("./ds2",0666);
	int fd=open("./ds2",O_RDWR);
	while(1)
	{
		dprintf(fd,"I'm D2\n");
		sleep(1);
	}
}
