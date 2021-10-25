#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

int main()
{
	char buff[30];
	strcpy(buff,"hello");
	mkfifo("./r1",0666);
	int r1=open("./r1",O_RDONLY);
	while(1)
	{
		scanf("%s",buff);
		printf("%s\n",buff);
	}
}