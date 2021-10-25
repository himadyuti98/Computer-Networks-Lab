#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[])
{
	//printf("p3\n");
	char s[20]="./",t[30];
	strcat(s,argv[1]);
	for(int i=3;i<argc;i++){
		strcat(s," ");
		strcat(s,argv[i]);
	}
	FILE * fd = popen(s,"w");
	int f = fileno(fd);
	dup2(f,1);
	scanf("%s",t);
	printf("%s\n",t);
}
