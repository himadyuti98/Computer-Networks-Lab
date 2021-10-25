#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

int main()
{
	mkfifo("./ds3",0666);
	int fd=open("./ds3",O_RDWR);
	while(1)
	{
		dprintf(fd,"I'm D3\n");
		sleep(1);
	}
}
