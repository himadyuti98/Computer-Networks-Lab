#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[])
{
	//printf("p4\n");
	char s[20]="./",t[30];
	strcat(s,argv[0]);
	for(int i=2;i<argc;i++){
		strcat(s," ");
		strcat(s,argv[i]);
	}
	scanf("%s",t);
	printf("%s\n",t);
}
