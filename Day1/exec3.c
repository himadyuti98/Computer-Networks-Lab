#include<stdio.h>
#include<unistd.h>
int main(int argc,char *argv[])
{

	printf("I am Exec3\n");
	argv[0]=NULL;
	execvp("./exec4",argv);
}
