#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<signal.h>
#include<sys/wait.h>
#include<string.h>

int main()
{
	int n;
	char c[2];
	scanf("%d",&n);
	char f[50];
	scanf("%s",f);
	int fd=open(f,O_RDONLY,0777);
	while(n--)
	{
		int a=read(fd,c,1);
		if(a==0)
			break;
		printf("%c",c[0]);
	}
	printf("\n***\n");
	int p = fork();
	if(p>0)
	{
		wait(NULL);
	}
	else
	{
		int stdinback=dup(0);
		dup2(fd,0);
		char s[10];// = (char *)stdinback;
		sprintf(s,"%d",stdinback);
		printf("%s\n",s);
		char * a[]={s,NULL};
		execvp("./file2",a);
	}
	return 0;
}
