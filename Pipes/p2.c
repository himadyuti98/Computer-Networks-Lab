#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#define size 25

int main()
{
	int pp[2];
	char s1[size],s2[size];
	pipe(pp);
	char f[size];
	scanf("%s",f);
	int fd = open(f,O_RDONLY,0777);
	dup2(pp[1],fd);
	dup2(pp[0],0);
	pid_t p=fork();
	if(p>0)
	{
		close(pp[0]);
		printf("Parent\n");
		scanf("%s",s1);
		write(pp[1],s1,strlen(s1)+1);
		wait(NULL);
	}
	else
	{
		char * a[]={NULL};
		execvp("./p3",a);
	}
	return 0;
}
