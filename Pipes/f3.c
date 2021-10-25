#include<stdio.h>
#include<unistd.h>

int main()
{
	FILE * fd = popen("./f4","r");
	char str[35];
	//fgets(str,35,fd);
	scanf("%s",str);
	printf("f3 %s",str);
}
