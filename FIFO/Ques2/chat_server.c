#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<poll.h>

int present(char a[][10],int n, char* f)
{
	for(int i=0;i<n;i++){
		if(strcmp(a[i],f)==0)
			return 1;}
	return 0;
}

int main()
{
	mkfifo("./spcl1",0666);
	mkfifo("./spcl2",0666);
	mkfifo("./spcl3",0666);
	mkfifo("./spcl4",0666);
	mkfifo("./spcl5",0666);
	struct pollfd fspcl[5];
	fspcl[0].fd=open("./spcl1",O_RDWR);
	fspcl[1].fd=open("./spcl2",O_RDWR);
	fspcl[2].fd=open("./spcl3",O_RDWR);
	fspcl[3].fd=open("./spcl4",O_RDWR);
	fspcl[4].fd=open("./spcl5",O_RDWR);
	for(int i=0;i<5;i++)
		fspcl[i].events=POLLIN;
	char b[5][20];
	char clients[5][20][10];
	int fd[5][20];
	int c=0;
	int d[5];
	memset(d,0,5*sizeof(int));
	while(c<20)
	{
		int s = poll(fspcl,5,-1);
		printf("s=%d",s);
		if(s>0){
		for(int e=0;e<5;e++)
		{
			if((fspcl[e].revents & POLLIN) &&(read(fspcl[e].fd,b[e],20)>0))
			{
				
				if(b[e][0]=='r' && b[e][1]=='e' && b[e][2]=='q')
				{
					char a[10];
					memset(a,'\0',10);
					int i;
					for(i=0;i<strlen(b[e])-3;i++)
					{
						a[i]=b[e][i+3];
					}
					a[i]='\0';
					a[5]='\0';a[6]='\0';
					printf("a=%s\n",a);
					if(present(clients[e],c,a)==0)
					{
						int q;
						for(q=0;q<strlen(a);q++)
							clients[e][c][q]=a[q];
						clients[e][c][q]='\0';
						mkfifo(a,0666);
						int v = open(a,O_RDWR);
						fd[e][d[e]++]=v;
						c++;
					}		
				}
				else
				{
					for(int i=0;i<d[e];i++)
					{
						write(fd[e][i],b[e],strlen(b[e]));
					}
				}
			}
		//fspcl[e].events=POLLIN;
		}
		}
	}
	return 0;
}
