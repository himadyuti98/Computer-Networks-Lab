#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/un.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<iostream>

using namespace std;

int main(int argc, char * argv[])
{
	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1])); //IPPROTO_RAW
	perror("socket");

	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.2");
	bind(rsfd,(struct sockaddr*)&server,sizeof(server));
	perror("bind");

	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int));//IP_HDRINCL
	perror("setsockopt");
	
		struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

	char *buff;
	buff = new char[8192];	
	struct iphdr *ip = (struct iphdr *)buff;
	char * msg = (char *)(buff+sizeof(struct iphdr));
	strcpy(msg,"hello");
	//scanf("%s",msg);
	ip->ttl = 20;
	ip->protocol = atoi(argv[1]);
	ip->version = 4;
	ip->tos = 1;
	ip->ihl = 5;
	ip->id = 0;
	ip->saddr = inet_addr("127.0.0.5");
	ip->daddr = inet_addr("127.0.0.1");
	ip->tot_len = sizeof(struct iphdr)+strlen(msg);
	printf("hoho\n");
	cout<<"------------------------\n";
	cout<<"Printing IP header....\n";
	cout<<"IP version:"<<(unsigned int)ip->version<<endl;
	cout<<"IP header length:"<<(unsigned int)ip->ihl<<endl;
	
	cout<<"Type of service:"<<(unsigned int)ip->tos<<endl;
	cout<<"Total ip packet length:"<<ntohs(ip->tot_len)<<endl;
	cout<<"Packet id:"<<ntohs(ip->id)<<endl;
	cout<<"Time to live :"<<(unsigned int)ip->ttl<<endl;
	cout<<"Protocol:"<<(unsigned int)ip->protocol<<endl;
	cout<<"Check:"<<ip->check<<endl;
	cout<<"Source ip:"<<inet_ntoa(*(in_addr*)&ip->saddr)<<endl;
	//printf("%pI4\n",&ip->saddr );
	cout<<"Destination ip:"<<inet_ntoa(*(in_addr*)&ip->daddr)<<endl;
	cout<<"End of IP header\n";
	cout<<"------------------------\n";
	
  sendto(rsfd,buff,ip->tot_len,0,(struct sockaddr*)&client,sizeof(client));
  perror("send");
  while(1);
}
