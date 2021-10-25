#include<stdio.h>
#include<unistd.h>

int main()
{
	char buff[10];
	while(1)
	{
		//sleep(1);
		scanf("%s",buff);
		printf("p6:%s\n",buff);
	}
}
