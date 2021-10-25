
#include<pcap.h>
#include<stdio.h>
#include<stdlib.h> // for exit()
#include<string.h> //for memset
 
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_ntoa()
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
 
void process_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
void process_ip_packet(const u_char * , int);
void print_ip_packet(const u_char * , int);
void print_tcp_packet(const u_char *  , int );
void print_udp_packet(const u_char * , int);
void print_icmp_packet(const u_char * , int );
void PrintData (const u_char * , int);
 
FILE *stdout; //to store the output
struct sockaddr_in source,dest;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j;



#define TCPSYN_LEN 20

/* Pseudoheader (Used to compute TCP checksum. Check RFC 793) */
struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
};

/*
    Generic checksum calculation function
*/
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


void PrintData (const u_char * data , int Size)
{
printf("printing data\n");

   //u_char *ptr=(u_char *)data;
 //  const char* S1 = reinterpret_cast<const char*>(data);
  // fprintf(stdout,"%s\n",S1);
    int i , j;
    for(i=0 ; i < Size ; i++)
    {
        if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
            fprintf(stdout , "         ");
            for(j=i-16 ; j<i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                    fprintf(stdout , "%c",(unsigned char)data[j]); //if its a number or alphabet
                 
                else fprintf(stdout , "."); //otherwise print a dot
            }
            fprintf(stdout , "\n");
        } 
         
        if(i%16==0) fprintf(stdout , "   ");
            fprintf(stdout , " %02X",(unsigned int)data[i]);
                 
        if( i==Size-1)  //print the last spaces
        {
            for(j=0;j<15-i%16;j++) 
            {
              fprintf(stdout , "   "); //extra spaces
            }
             
            fprintf(stdout , "         ");
             
            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128) 
                {
                  fprintf(stdout , "%c",(unsigned char)data[j]);
                }
                else
                {
                  fprintf(stdout , ".");
                }
            }
             
            fprintf(stdout ,  "\n" );
        }
    }
}



  pcap_t *handle; //Handle of the device that shall be sniffed
  

int main(int argc,char *argv[])
{
    if(argc<=1)
    {
        printf("error format : devname");
    }

    pcap_if_t *device;
    bpf_u_int32 net,mask;
    struct bpf_program filter;

    char errbuf[100] , *devname , devs[100][100];
     
    devname = argv[1];
     
    //Open the device for sniffing
    printf("Opening device %s for sniffing ... " , devname);
    handle = pcap_open_live(devname , 65535 , 1 , 0 , errbuf);

      if (handle == NULL) 
    {
        fprintf(stderr, "Couldn't open device %s : %s\n" , devname , errbuf);
        exit(1);
    }


    pcap_lookupnet(devname,&net,&mask,errbuf);

    pcap_compile(handle,&filter,"src 192.168.43.150 && udp",1,mask);

    pcap_setfilter(handle,&filter);
     
  
    printf("Done\n");
      
    //Put the device in sniff loop
    pcap_loop(handle , 0 , process_packet , NULL); 
    return 0;   
}



void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
    int size = header->len;

    printf("got packet \n");


    struct iphdr *iph = (struct iphdr *)(buffer  + sizeof(struct ethhdr) );
    
    struct udphdr *udphdr=(struct udphdr*)(buffer + iph->ihl*4 + sizeof(struct ethhdr));

    int header_size =  sizeof(struct ethhdr) + iph->ihl*4 + 8;
    PrintData(buffer + header_size , size - header_size );
    char *buff = (char *)(buffer + header_size);
    strcpy(buff,"change");
    udphdr->check=0;

    PrintData(buffer + header_size , size - header_size );

    u_char *newbuffer=malloc(65536);
    strcpy(newbuffer,buffer);
   
  /* Fill the pseudoheader so we can compute the TCP checksum*/
  
  /*  char *pseudogram;
    struct pseudo_header psh;
    psh.source_address =  iph->saddr;
    psh.dest_address = iph->daddr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data) );
    
    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
    pseudogram = malloc(psize);
    
    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + strlen(data));
    
*/
    
  //  tcph->check = csum ((unsigned short *) (buffer+sizeof(struct ethhdr)+iph->ihl*4), iph->tot_len-iph->ihl*4);

    int x = sizeof(buffer);
    if(pcap_sendpacket(handle,buffer,1234)<0)
    {
        perror("cant send packet\n");
    }
    else
    printf("packet sent\n");
}
