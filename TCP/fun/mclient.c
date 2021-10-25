#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

struct array
{
	int a[20];
	int n;
};

int main(int argc, char * argv[])
{
	int sfd= socket(AF_INET, SOCK_STREAM, 0);
	printf("%d\n", sfd);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(atoi(argv[1]));
	
	connect(sfd,(struct sockaddr*)&saddr, sizeof(saddr));

	printf("connected\n");

	struct array arr;
	scanf("%d",&arr.n);
	for(int i=0;i<arr.n;i++)
		scanf("%d",&arr.a[i]);

	send(sfd,(void*)&arr,sizeof(arr),0);
	while(recv(sfd,&arr,sizeof(arr),0)<=0);
	for(int i=0;i<arr.n;i++)
		printf("%d ",arr.a[i]);
	printf("\n");
}
