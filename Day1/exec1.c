#include<stdio.h>
#include<unistd.h>
int main(int argc,char *argv[])
{

	printf("I am Exec1\n");
	argv[0]=NULL;
	execvp("./exec2",argv);
}
