#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[])
{
	char* a[20];
	for(int i=2;i<argc;i++)
		a[i-2]=argv[i];
	a[argc-1]=NULL;
	char s[20]="./";
	strcat(s,argv[1]);
	execvp(s,a);
	return 0;
}
