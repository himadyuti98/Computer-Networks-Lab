#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<stdlib.h>

int sfd[3],nsfd;
char buff[30];

int main()
{
	struct pollfd fd[3];
	for(int i=0;i<3;i++)
		sfd[i]= socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int port = 5050;
	for(int i=0;i<3;i++)
	{
		saddr.sin_port = htons(port++);
		bind(sfd[i], (struct sockaddr*)&saddr, sizeof(saddr));
		listen(sfd[i],SOMAXCONN);
		fd[i].fd=sfd[i];
		fd[i].events=POLLIN;
	}

	while(1)
	{
		int s = poll(fd,3,-1);
		if(s>0){
			for(int i=0;i<3;i++){
				if(fd[i].revents & POLLIN)
				{
					nsfd = accept(sfd[i],NULL,NULL);
					int c = fork();
					if(c==0)
					{
						close(sfd[0]);
						close(sfd[1]);
						close(sfd[2]);
						strcpy(buff,"hello");
						write(nsfd,buff,strlen(buff));
						memset(buff,'\0',30);
						char s[4]="./s";
						char t[2];
						sprintf(t,"%d",i+1);
						strcat(s,t);
						sprintf(t,"%d",nsfd);
						printf("%s\n",t);
						char * a[] = {"",NULL};
						a[0]=t;
						dup2(nsfd,2);
						//dup2(nsfd,1);
						printf("s=%s\n", s);
						execvp(s,a);
					}
					else
					{
						close(nsfd);
					}
				}
			}
		
		
	}
	}

}



//system("gnome-terminal -e \'sh -c \"echo test; execvp(s,a); exec bash\"'");
