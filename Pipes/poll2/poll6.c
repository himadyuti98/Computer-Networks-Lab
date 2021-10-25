#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	char buffer[16];
	read(0,buffer,17);
	printf("%s",buffer);
	return 0;
}
