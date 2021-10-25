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
	printf("Client4 received: %s\n",buff1);
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
	mkfifo("./cl4",0666);
	fd=open("./cl4",O_RDWR);
	
	char s[20]="req./cl4";
	s[8]='\0';s[9]='\0';
	int t;
	printf("Enter the group you want to join. Options: 1,2,3,4,5\n");
	scanf("%d",&t);
	if(t==1){
		mkfifo("./spcl1",0666);
		fspcl=open("./spcl1",O_RDWR);}
	else if(t==2){
		mkfifo("./spcl2",0666);
		fspcl=open("./spcl2",O_RDWR);}
	else if(t==3){
		mkfifo("./spcl3",0666);
		fspcl=open("./spcl3",O_RDWR);}
	else if(t==4){
		mkfifo("./spcl4",0666);
		fspcl=open("./spcl4",O_RDWR);}
	else {
		mkfifo("./spcl5",0666);
		fspcl=open("./spcl5",O_RDWR);}
	printf("%d %s",fspcl,s);
	write(fspcl,s,strlen(s));
	
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(r,NULL);
	pthread_join(w,NULL);	
	
		
	return 0;
}
