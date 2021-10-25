#include<stdio.h>
#include<unistd.h>
#include<signal.h>
void fn()
{
	printf("I won't terminate\n");
}
int main()
{
	signal(SIGINT,fn);
	while(1);	
	return 0;
}
