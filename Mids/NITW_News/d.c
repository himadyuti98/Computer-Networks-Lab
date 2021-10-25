#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
	char buff[30];
	mkfifo("./dw",0666);
	int w=open("./dw",O_RDWR);
	int r;
	while(1)
	{
		read(w,buff,30);
		if(buff[0]=='.' && buff[1]=='/' && buff[2]=='r' && buff[3]=='e' && buff[4]=='p')
		{
			char doc[40];
			strcpy(doc,buff);
			strcat(doc,"doc.txt");
			mkfifo(buff,0666);
			r=open(buff,O_RDWR);
			int fd = open(doc,O_CREAT|O_RDWR);
			while(buff[0]!='x')
			{
				memset(buff,'\0',30);
				read(r,buff,30);
				printf("%s\n",buff);
				write(fd,buff,strlen(buff));
			}
		}
	}
}
