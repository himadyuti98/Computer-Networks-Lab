#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>

char buff1[100], buff2[100];
int fspcl,fd;

void * reader(void * f)
{
	while(1){
	read(fd,buff1,100);
	printf("Client1 received: %s\n",buff1);
	memset(buff1,'\0',100);
	sleep(1);}
}

void * writer(void * f)
{
	while(1)
	{
		//printf("cl2 Enter message: ");
		scanf("%s",buff2);
		char s[20]="./cl1%";
		strcat(s,buff2);
		write(fspcl,s,sizeof(s));
		sleep(1);
	}
}
int main()
{
	mkfifo("./cl1",0666);
	fd=open("./cl1",O_RDWR);
	mkfifo("./spcl",0666);
	fspcl=open("./spcl",O_RDWR);
	
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(r,NULL);
	pthread_join(w,NULL);	
	
		
	return 0;
}