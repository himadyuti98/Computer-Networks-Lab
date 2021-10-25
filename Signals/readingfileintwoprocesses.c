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
	char c;
	scanf("%d",&n);
	char f[50];
	scanf("%s",f);
	FILE *fp = fopen (f, "r");
	while(n--)
	{
		c=fgetc(fp);
		if(c==EOF)
			break;
		printf("%c",c);
	}
	printf("\n***\n");
	int p = fork();
	if(p>0)
	{
		wait(NULL);
	}
	else
	{
		while(1)
		{
			c=fgetc(fp);
			if(c==EOF)
				break;
			printf("%c",c);
		}
		fclose(fp);
	}
	return 0;
}
