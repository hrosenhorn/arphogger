#include <stdio.h>
#include "ethernet.h"

#ifndef __ARP_H__
#define __ARP_H__

typedef struct
{
  u16 hardware_type;
  u16 protocol_type;
  u8 hardware_length;
  u8 protocol_length;
  u16 operation;
  u8 source_hardware_address[ETHERNET_ADDRESS_LENGTH];
  u32 source_protocol_address;
  u8 target_hardware_address[ETHERNET_ADDRESS_LENGTH];
  u32 target_protocol_address;
} __attribute__ ((packed)) ethernet_arp_type;

#endif
