#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<pthread.h>

void * runner(void * f)
{
	int * p = (int *)f;
	int ds=p[0];
	int fd = p[1];
	char buff[50];
	while(1)
	{
		read(ds,buff,50);
		//printf("buff=%s\n",buff);
		printf("fd=%d\n",fd);
		write(fd,buff,strlen(buff));
	}
}

int main()
{
	mkfifo("./spcl",0666);
	int fspcl=open("./spcl",O_RDWR);
	char b1[20],b2[100];
	char clients[20][10];
	int d[3],ds[3];
	memset(d,0,3*sizeof(int));
	int fd[20];
	int c=0;
	pthread_t pt[20];
	while(c<20)
	{
		if(read(fspcl,b1,20)>0)
		{
			char a[10],b[2];
			int i;
			for(i=0;i<=4;i++)
				a[i]=b1[i];
			a[5]='\0';
			mkfifo(a,0666);
			b[0]=b1[6];b[1]='\0';
			int q = atoi(b);
			printf("q=%d\n",q);
			if(d[q-1]==1)
			{
				/*char w[20];
				read(ds[q-1],w,20);
				int f=open(a,O_RDWR);
				write(f,w,strlen(w));*/
				int w[2];
				w[0]=ds[q-1];
				w[1]=open(a,O_RDWR);
				pthread_create(&pt[c++],NULL,&runner,(void*)w);
			}
			else
			{
				if(q==1)
				{
					mkfifo("./ds1",0666);
					ds[0]=open("./ds1",O_RDWR);
					d[0]=1;
				}
				else if(q==2)
				{
					mkfifo("./ds2",0666);
					ds[1]=open("./ds2",O_RDWR);
					d[1]=1;
				}
				else
				{
					mkfifo("./ds3",0666);
					ds[2]=open("./ds3",O_RDWR);
					d[2]=1;
				}
				/*char w[20];
				read(ds[q-1],w,20);
				int f=open(a,O_RDWR);
				write(f,w,strlen(w));*/
				
				int w[2];
				w[0]=ds[q-1];
				w[1]=open(a,O_RDWR);
				pthread_create(&pt[c++],NULL,&runner,(void*)w);
			}
		}
	}
	for(int i=0;i<c;i++)
	{
		pthread_join(pt[i],NULL);
	}
}
