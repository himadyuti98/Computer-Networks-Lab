#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[])
{
	char c[2];
	printf("second program\n");
	int fd=dup(0);
	printf("%s",argv[0]);
	dup2(atoi(argv[0]),0);
	while(1)
	{
		int a=read(fd,c,1);
		if(a==0)
			break;
		printf("%c",c[0]);
	}
	scanf("%s",c);
	printf("*****%s",c);
	return 0;
}
