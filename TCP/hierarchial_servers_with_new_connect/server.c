#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<sys/wait.h>
#include<stdlib.h>

int sfd,nsfd,ssfd;
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
		perror("error :");
		int c = fork();
		if(c==0)
		{
			close(sfd);
			printf("nsfd=%d\n",nsfd );
			read(nsfd,buff,30);
			printf("%s\n", buff);
			ssfd= socket(AF_INET, SOCK_STREAM, 0);
			printf("ssfd=%d\n",ssfd );
			struct sockaddr_in saddr;
			saddr.sin_family=AF_INET;
			saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
			saddr.sin_port = htons(5500+atoi(buff));
	
			int i=connect(ssfd,(struct sockaddr*)&saddr, sizeof(saddr));
			printf("connected %d\n",i);
			//read(ssfd,buff,20);
			//printf("%s\n", buff);
			/*sprintf(buff,"%d",nsfd);
			send(ssfd,buff,strlen(buff));
			sprintf(buff,"%d",ssfd);
			send(nsfd,buff,strlen(buff));*/
			//dup2(nsfd,ssfd);
			//close(ssfd);
			/*pthread_t r,w;
			pthread_create(&w,NULL,&writer,NULL);
			pthread_create(&r,NULL,&reader,NULL);
			pthread_join(r,NULL);
			pthread_join(w,NULL);	*/
			read(ssfd,buff,30);
			write(nsfd,buff,30);
			exit(0);
		}
		else
		{
			close(nsfd);
			wait(NULL);
		}
	}


}

