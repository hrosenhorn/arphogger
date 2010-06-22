#include <stdio.h>

#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#define ETHERNET_HEADER_LENGTH          14
#define ETHERNET_ADDRESS_LENGTH 	6
#define ETHERNET_PROTOCOL_IPV4          8
#define ETHERNET_PROTOCOL_ARP		0x0806

#define IPV4_HEADER_LENGTH 		20
#define IPV4_PROTOCOL_ICMP		1
#define IPV4_PROTOCOL_TCP		6
#define IPV4_PROTOCOL_UDP		17

#define TCP_HEADER_LENGTH		18

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

typedef struct
{
  u8 destination_address[ETHERNET_ADDRESS_LENGTH];
  u8 source_address[ETHERNET_ADDRESS_LENGTH];
  u16 protocol_type;
  //u8 data[0];
} __attribute__ ((packed)) ethernet_header_type;




#endif
