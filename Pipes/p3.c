#include<unistd.h>
#include<stdio.h>
#define size 25

int main()
{
	char s[size];
	printf("Child\n");
	read(0,s,size+1);
	printf("%s\n",s);
	return 0;
}
