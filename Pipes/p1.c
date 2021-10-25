#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#define size 25

int main()
{
	int pp1[2],pp2[2];
	char s1[size],s2[size],s3[size],s4[size];
	pipe(pp1);
	pipe(pp2);
	pid_t p=fork();
	if(p>0)
	{
		close(pp1[0]);
		close(pp2[1]);
		printf("Parent\n");
		scanf("%s",s1);
		write(pp1[1],s1,strlen(s1)+1);
		sleep(1);
		read(pp2[0],s3,size);
		printf("Parent\t%s\n",s3);
		wait(NULL);
	}
	else
	{
		sleep(1);
		close(pp2[0]);
		close(pp1[1]);
		printf("Child\n");
		scanf("%s",s2);
		write(pp2[1],s2,strlen(s2)+1);
		sleep(1);
		read(pp1[0],s4,size);
		printf("Child\t%s\n",s4);
	}
	return 0;
}
