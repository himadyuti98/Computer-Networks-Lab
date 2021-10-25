#include<stdio.h>
#include<unistd.h>

int main()
{
	FILE * fd = popen("./f2","w");
	fputs("hello\n",fd);
}
