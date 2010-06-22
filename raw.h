#include "arp.h"

#ifndef __RAW_H__
#define __RAW_H__

#define ETHNAME "eth0"
#define ETHMAC "mac address of eth0 here"
#define GATEWAYIP "your ip here"

// Lazy? Yes!

typedef struct
{
   ethernet_header_type ethernet_header;
   //unsigned char data[0];
   ethernet_arp_type arp_packet;
   //unsigned char data[0];
} __attribute__ ((packed)) packet_type;


#endif
