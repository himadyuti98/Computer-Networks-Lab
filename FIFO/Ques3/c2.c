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
	printf("%s\n",buff1);
	memset(buff1,'\0',100);
	sleep(1);}
}

void * writer(void * f)
{
	while(1)
	{
		//printf("cl2 Enter message: ");
		scanf("%s",buff2);
		write(fspcl,buff2,sizeof(buff2));
		sleep(1);
	}
}
int main()
{
	mkfifo("./cl2",0666);
	fd=open("./cl2",O_RDWR);
	mkfifo("./spcl",0666);
	fspcl=open("./spcl",O_RDWR);
	char s[20]="./cl2$";
	char t[2];
	printf("Enter the data service you want. Options: 1,2,3\n");
	scanf("%s",t);
	strcat(s,t);
	write(fspcl,s,strlen(s));
	pthread_t r,w;
	//pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(r,NULL);
	//pthread_join(w,NULL);	
	
		
	return 0;
}
