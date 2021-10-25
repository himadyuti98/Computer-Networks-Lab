#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/INET_Addr.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

int main(int argc, char const *argv[])
{
	int portno = atoi(argv[1]);
	ACE_INET_Addr addr(portno,INADDR_LOOPBACK);
	ACE_SOCK_Connector con;
	ACE_SOCK_Stream sfd;
	if(con.connect(sfd,addr)<0)
	{
		perror("Could not connect");
		exit(0);
	}
	else
	{
		printf("Connected sucessfully..\n");
	}
	char buffer[50];int sz;char a[50];
	while(1)
	{
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		printf("Read from keyboard: %s\n",buffer);
		if(sfd.send_n(buffer,50)<0)
		{
			perror("Could not send");
		}	
		else
		{
			printf("Sent succesfully\n");
		}
		/*if((sz = sfd.recv_n(a,50))<0)
		{
			///perror("Could not send");
		}
		else
		{
			a[sz] = '\0';
			printf("No of vowels in %s :%s\n",buffer,a);
		}*/
	}
	return 0;
}
