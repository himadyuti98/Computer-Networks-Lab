#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<stdlib.h>

int sfd,nsfd;
char buff[30];

int main()
{
	sfd= socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(5050);
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd,SOMAXCONN);

	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		int c = fork();
		if(c==0)
		{
			close(sfd);
			read(nsfd,buff,30);
			char s[4]="./s";
			strcat(s,buff);
			char * a[] = {"",NULL};
			//a[0]=t;
			dup2(nsfd,2);
			printf("s=%s\n", s);
			execvp(s,a);
		}
		else
		{
			close(nsfd);
		}
	}


}

