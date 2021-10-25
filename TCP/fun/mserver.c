#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<math.h>


struct array
{
	int a[20];
	int n;
};

int sfd[2],nsfd;


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
	sfd[0]= socket(AF_INET, SOCK_STREAM, 0);
	sfd[1]= socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(5051);
	int f = bind(sfd[0], (struct sockaddr*)&saddr, sizeof(saddr));
	saddr.sin_port = htons(5052);
	f = bind(sfd[1], (struct sockaddr*)&saddr, sizeof(saddr));
	printf("%d\n",f);
	listen(sfd[0],SOMAXCONN);
	listen(sfd[1],SOMAXCONN);

	fd_set r;
	int m = fmax(sfd[0],sfd[1]);

	while(1)
	{
		FD_ZERO(&r);
		FD_SET(sfd[0],&r);
		FD_SET(sfd[1],&r);
		
		int s = select(m+1,&r,NULL,NULL,NULL);
		if(s>0)
		{
			for(int i=0;i<2;i++)
			{
				if(FD_ISSET(sfd[i],&r))
				{
					nsfd = accept(sfd[i],NULL,NULL);
					int c = fork();
					if(c==0)
					{
						close(sfd[i]);
						printf("accepted\n");
						struct array arr;
						recv(nsfd,&arr,sizeof(arr),0);
						printf("received\n");
						if(i==0)
							arr=bubblesort(arr);
						else if(i==1)
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
				
			}
			
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