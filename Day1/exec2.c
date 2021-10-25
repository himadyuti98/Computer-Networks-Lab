#include<stdio.h>
#include<unistd.h>
int main(int argc,char *argv[])
{

	printf("I am Exec2\n");
	argv[0]=NULL;
	execvp("./exec3",argv);
}
