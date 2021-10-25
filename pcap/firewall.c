#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>           // close()
#include <string.h>           // strcpy, memset()
 
#include <netinet/ip.h>       // IP_MAXPACKET (65535)
#include <sys/types.h>        // needed for socket(), uint8_t, uint16_t
#include <sys/socket.h>       // needed for socket()
#include <linux/if_ether.h>   // ETH_P_ARP = 0x0806, ETH_P_ALL = 0x0003
#include <net/ethernet.h>
 
#include <errno.h>            // errno, perror()
 
// Define an struct for IP header

 
// Define some constants.
#define ARPOP_REPLY 2         // Taken from <linux/if_arp.h>
 
// Function prototypes
uint8_t *allocate_ustrmem (int);
 
int
main (int argc, char **argv)
{
  int i, sd, status;
  uint8_t *ether_frame;
  
  // Allocate memory for various arrays.
  ether_frame = allocate_ustrmem (IP_MAXPACKET);
 
  // Submit request for a raw socket descriptor.
  if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {
    perror ("socket() failed ");
    exit (EXIT_FAILURE);
  }
 
  // Listen for incoming ethernet frame from socket sd.
  // We expect an ARP ethernet frame of the form:
  //     MAC (6 bytes) + MAC (6 bytes) + ethernet type (2 bytes)
  //     + ethernet data (ARP header) (28 bytes)
  // Keep at it until we get an ARP reply.
  while (((((ether_frame[12]) << 8) + ether_frame[13]) != ETH_P_ARP) || (ntohs (arphdr->opcode) != ARPOP_REPLY)) {
    if ((status = recv (sd, ether_frame, IP_MAXPACKET, 0)) < 0) {
      if (errno == EINTR) {
        memset (ether_frame, 0, IP_MAXPACKET * sizeof (uint8_t));
        continue;  // Something weird happened, but let's try again.
      } else {
        perror ("recv() failed:");
        exit (EXIT_FAILURE);
      }
    }
  }
  close (sd);
 
  // Print out contents of received ethernet frame.
  printf ("\nEthernet frame header:\n");
  printf ("Destination MAC (this node): ");
  for (i=0; i<5; i++) {
    printf ("%02x:", ether_frame[i]);
  }
  printf ("%02x\n", ether_frame[5]);
  printf ("Source MAC: ");
  for (i=0; i<5; i++) {
    printf ("%02x:", ether_frame[i+6]);
  }
  printf ("%02x\n", ether_frame[11]);
  // Next is ethernet type code (ETH_P_ARP for ARP).
  // http://www.iana.org/assignments/ethernet-numbers
  char code[20];
  sprintf (code, "%u", ((ether_frame[12]) << 8) + ether_frame[13]);
  if(strcmp(code,"2054")!=0)
  {
    printf("ip packet\n");
    struct iphdr *iph = (struct iphdr*)(ether_frame + sizeof(struct ethhdr));
    printf("\n");
    printf("IP Header\n");
    printf("   |-IP Version        : %d\n",(unsigned int)iph->version);
    printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    printf("   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    printf("   |-Identification    : %d\n",ntohs(iph->id));
    printf("   |-TTL      : %d\n",(unsigned int)iph->ttl);
    printf("   |-Protocol : %d\n",(unsigned int)iph->protocol);
    printf("   |-Checksum : %d\n",ntohs(iph->check));
    printf("   |-Source IP        : %s\n" , inet_ntoa(iph->saddr) );
    printf("   |-Destination IP   : %s\n" , inet_ntoa(iph->daddr) );
    if(strcmp(inet_ntoa(iph->saddr),restricted_ip)==0 && strcmp(inet_ntoa(iph->daddr),victim_ip)==0)
    {

    }
    else
    {
      ether_frame[0] = ;
      ether_frame[1] = ;
      ether_frame[2] = ;
      ether_frame[3] = ;
      ether_frame[4] = ;
      ether_frame[5] = ;
      if ((bytes = sendto (sd, ether_frame, frame_length, 0, (struct sockaddr *) &device, sizeof (device))) <= 0) {
        perror ("sendto() failed");
        exit (EXIT_FAILURE);
      }
    }

  }
  
  /*printf ("Ethernet type code (2054 = ARP): %u\n", ((ether_frame[12]) << 8) + ether_frame[13]);
  printf ("\nEthernet data (ARP header):\n");
  printf ("Hardware type (1 = ethernet (10 Mb)): %u\n", ntohs (arphdr->htype));
  printf ("Protocol type (2048 for IPv4 addresses): %u\n", ntohs (arphdr->ptype));
  printf ("Hardware (MAC) address length (bytes): %u\n", arphdr->hlen);
  printf ("Protocol (IPv4) address length (bytes): %u\n", arphdr->plen);
  printf ("Opcode (2 = ARP reply): %u\n", ntohs (arphdr->opcode));
  printf ("Sender hardware (MAC) address: ");
  for (i=0; i<5; i++) {
    printf ("%02x:", arphdr->sender_mac[i]);
  }
  printf ("%02x\n", arphdr->sender_mac[5]);
  printf ("Sender protocol (IPv4) address: %u.%u.%u.%u\n",
    arphdr->sender_ip[0], arphdr->sender_ip[1], arphdr->sender_ip[2], arphdr->sender_ip[3]);
  printf ("Target (this node) hardware (MAC) address: ");
  for (i=0; i<5; i++) {
    printf ("%02x:", arphdr->target_mac[i]);
  }
  printf ("%02x\n", arphdr->target_mac[5]);
  printf ("Target (this node) protocol (IPv4) address: %u.%u.%u.%u\n",
    arphdr->target_ip[0], arphdr->target_ip[1], arphdr->target_ip[2], arphdr->target_ip[3]);*/
 
  free (ether_frame);
 
  return (EXIT_SUCCESS);
}
 
// Allocate memory for an array of unsigned chars.
uint8_t *
allocate_ustrmem (int len)
{
  void *tmp;
 
  if (len <= 0) {
    fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_ustrmem().\n", len);
    exit (EXIT_FAILURE);
  }
 
  tmp = (uint8_t *) malloc (len * sizeof (uint8_t));
  if (tmp != NULL) {
    memset (tmp, 0, len * sizeof (uint8_t));
    return (tmp);
  } else {
    fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_ustrmem().\n");
    exit (EXIT_FAILURE);
  }
}