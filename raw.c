#include <stdio.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ethernet.h"
#include "ipv4.h"
#include "arp.h"
#include "raw.h"

extern struct sockaddr_ll saddr;

int raw_init (char *device)
{
  struct ifreq ifr;
  int raw_socket;
  int ifindex;

  memset (&ifr, 0, sizeof (struct ifreq));
  memset (&saddr, 0, sizeof (struct sockaddr_ll));

  /* Open A Raw Socket */
  if ((raw_socket = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 1)
  {
    printf ("ERROR: Could not open socket, Got #?\n");
    exit (1);
  }

  bind(raw_socket, (struct sockaddr *)&saddr, sizeof(struct sockaddr_ll));

  /* Set the device to use */
  strcpy (ifr.ifr_name, device);

  /* Get the current flags that the device might have */
  if (ioctl (raw_socket, SIOCGIFFLAGS, &ifr) == -1)
  {
   perror ("Error: Could not retrive the flags from the device.\n");
   exit (1);
  }

  /* Set the old flags plus the IFF_PROMISC flag */
  ifr.ifr_flags |= IFF_PROMISC;
  if (ioctl (raw_socket, SIOCSIFFLAGS, &ifr) == -1)
  {
    perror ("Error: Could not set flag IFF_PROMISC");
    exit (1);
  }

  /* Configure the device */

  if (ioctl (raw_socket, SIOCGIFINDEX, &ifr) < 0)
  {
     perror ("Error: Error getting the device index.\n");
     exit (1);
  }
  ifindex = ifr.ifr_ifindex;
  saddr.sll_family = AF_PACKET;
  saddr.sll_ifindex = ifindex;
  saddr.sll_protocol = htons (ETH_P_ALL); /* Now we will get ALL packets */

  bind(raw_socket, (struct sockaddr *)&saddr, sizeof(struct sockaddr_ll));

  return raw_socket;
}

int raw_parse (int raw_socket, unsigned char *data)
{
  int received_bytes;
  unsigned short ethernet_proto;
  unsigned short ipv4_proto,port_source,port_destination;
  int counter;

  packet_type *packet;
  unsigned char *pointer;
  struct in_addr in;

  memset (data, 0, 2047);

  if ((received_bytes = recv (raw_socket, data, 2047, 0)) == -1)
  {
     fprintf (stderr,"Error reading on raw socket...exiting\n");
     exit (1);
  }

  packet = (packet_type *) data;

  memcpy (&ethernet_proto, &data[12], sizeof (ethernet_proto));

  if (ethernet_proto == ntohs (ETHERNET_PROTOCOL_ARP))
  {
    packet = (packet_type *) data;

    if (inet_addr (GATEWAYIP) == packet->arp_packet.source_protocol_address)
    {
      unsigned char source_ip[16];
      unsigned char destination_ip[16];

      in.s_addr = packet->arp_packet.source_protocol_address;
      memcpy (&source_ip, inet_ntoa (in), sizeof(source_ip));
      in.s_addr = packet->arp_packet.target_protocol_address;
      memcpy (&destination_ip, inet_ntoa (in), sizeof(destination_ip));

      fprintf (stderr, "%s is asking us for ARP, he wants to know who has %s. We should respond to MAC: %s.\n", source_ip, destination_ip, ether_ntoa (packet->arp_packet.source_hardware_address));
      packet_type newpacket;
      memset (&newpacket, 0, sizeof (newpacket));

      // The Ethernet header
      memcpy (newpacket.ethernet_header.destination_address, packet->ethernet_header.source_address, ETHERNET_ADDRESS_LENGTH);
      memcpy (newpacket.ethernet_header.source_address, (void *) ether_aton (ETHMAC), ETHERNET_ADDRESS_LENGTH);
      newpacket.ethernet_header.protocol_type = packet->ethernet_header.protocol_type;

      // The ARP package
      newpacket.arp_packet.hardware_type = packet->arp_packet.hardware_type;
      newpacket.arp_packet.protocol_type = packet->arp_packet.protocol_type;
      newpacket.arp_packet.hardware_length = packet->arp_packet.hardware_length;
      newpacket.arp_packet.protocol_length = packet->arp_packet.protocol_length;
      newpacket.arp_packet.operation = htons (2); // ARP reply

      // copy the mac
      memcpy (newpacket.arp_packet.target_hardware_address, packet->ethernet_header.source_address, ETHERNET_ADDRESS_LENGTH);
      memcpy (newpacket.arp_packet.source_hardware_address, (void *) ether_aton (ETHMAC), ETHERNET_ADDRESS_LENGTH);

      // copy the ip
      memcpy (&newpacket.arp_packet.source_protocol_address, &packet->arp_packet.target_protocol_address, sizeof (packet->arp_packet.source_protocol_address));
      memcpy (&newpacket.arp_packet.target_protocol_address, &packet->arp_packet.source_protocol_address, sizeof (packet->arp_packet.source_protocol_address));


      write(raw_socket, &newpacket, sizeof (packet_type));
    }
  }

  return 0;
}
