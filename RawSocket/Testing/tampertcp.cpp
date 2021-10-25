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
#include<netinet/tcp.h>

using namespace std;


unsigned short csum(unsigned short *ptr,int nbytes) 
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }

    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
    
    return(answer);
}


int main(int argc, char * argv[])
{
	int rsfd=socket(AF_INET,SOCK_RAW,6); //IPPROTO_RAW
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
	socklen_t clilen=sizeof(client);
	
	char *buff;
	buff = new char[8192];	
	
	recvfrom(rsfd,buff,50,0,(struct sockaddr*)&client,(socklen_t*)&clilen);
	perror("recv");
	char * msg = (char *)(buff+sizeof(struct iphdr)+sizeof(struct tcphdr));
	
	struct iphdr *ip = (struct iphdr *)buff;
	struct tcphdr *tcph = (struct tcphdr *)ip+4*ip->ihl;
	ip->daddr = inet_addr("127.0.0.1");
	ip->tot_len = sizeof(struct iphdr)+strlen(msg);
	tcph->seq = 2*tcph->seq;
	swap(tcph->source,tcph->dest);
    tcph->check = 0;
    //tcph->check =  ~(~tcph->check + ~(tcph->seq/2) + tcph->seq);//csum((unsigned short *)tcph,ip->tot_len-4*ip->ihl);

	//scanf("%s",msg);
	
	
  sendto(rsfd,buff,ip->tot_len,0,(struct sockaddr*)&client,sizeof(client));
  perror("send");
 // while(1);
}
