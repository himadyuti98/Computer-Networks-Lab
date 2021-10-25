#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>


struct array
{
	int service;
	int a[20];
	int n;
};

int sfd,nsfd;


struct array bubblesort(struct array arr)
{
	for(int i=0;i<arr.n;i++)
	{
		for(int j=1;j<arr.n;j++)
		{
			if(arr.a[j]<arr.a[j-1])
			{
				int t = arr.a[j];
				arr.a[j]=arr.a[j-1];
				arr.a[j-1]=t;
			}
		}
	}
	return arr;
}

void insertionsort(struct array *arr)
{
	for (int i = 0; i < arr->n-1; ++i)
	{
		int j=i+1;
		int t=arr->a[j];
		while(arr->a[j-1]>t && j-1>=0)
		{
			int x = arr->a[j];
			arr->a[j]=arr->a[j-1];
			arr->a[j-1]=x;
			j--;
		}
		//arr->a[i]=t;
		for(int k=0;k<arr->n;k++)
			printf("%d ",arr->a[k]);
		printf("\n");
	}
	printf("huhu\n");
	//return arr;
}

int main()
{
	sfd= socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(5051);
	int f = bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	printf("%d\n",f);
	listen(sfd,SOMAXCONN);

	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		int c = fork();
		if(c==0)
		{
			close(sfd);
			printf("accepted\n");
			struct array arr;
			recv(nsfd,&arr,sizeof(arr),0);
			printf("received\n");
			if(arr.service==1)
				arr=bubblesort(arr);
			else if(arr.service==2)
				insertionsort(&arr);
			printf("sorted\n");
			send(nsfd,(void*)&arr,sizeof(arr),0);
			printf("sent\n");
			exit(0);
		}
		else
		{
			close(nsfd);
		}
	}

	

	//char buff[50];
	/*strcpy(buff,"hello");
	write(nsfd,buff,strlen(buff));
	memset(buff,'\0',50);
	pthread_t r,w;
	pthread_create(&w,NULL,&writer,NULL);
	pthread_create(&r,NULL,&reader,NULL);
	pthread_join(r,NULL);
	pthread_join(w,NULL);	*/
}