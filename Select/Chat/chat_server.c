#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

int present(char a[][10],int n, char* f)
{
	for(int i=0;i<n;i++){
		if(strcmp(a[i],f)==0)
			return 1;}
	return 0;
}

int main()
{
	printf("return %d\n",mkfifo("./spcl",0666));
	int fspcl=open("./spcl",O_RDWR);
	char b1[20],b2[100];
	char clients[20][10];
	int fd[20];
	int c=0;
	while(c<20)
	{
		if(read(fspcl,b1,20)>0){
		printf("*********%s\n",b1);
		char a[10]="./",b[15];
		int i;
		for(i=0;i<strlen(b1);i++)
		{
			if(b1[i]=='%'){
				a[i]='\0';i++;
				break;}
			a[i]=b1[i];
		}
		int j;
		for(j=0;i<strlen(b1);i++,j++)
			b[j]=b1[i];
		b[j]='\0';
		printf("a=%s\n",a);
		if(present(clients,c,a)==0){
		int q;
		for(q=0;q<strlen(a);q++)
			clients[c][q]=a[q];
		clients[c][q]='\0';
		mkfifo(a,0666);
		int v = open(a,O_RDWR);
		fd[c++]=v;
		//close(v);
		}
		for(int i=0;i<c;i++)
		{
			printf("&&&&%d\n",fd[i]);
			write(fd[i],b,strlen(b));
		}		
		}
	}
	
	return 0;
}
