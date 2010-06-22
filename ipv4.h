#ifndef __IPV4_H__
#define __IPV4_H__

#define u32 unsigned int
#define u16 unsigned short
#define u8  unsigned char

typedef struct
 {
   /* Header length, in u32:s. */

   u8 header_length: 4;
   u8 version: 4;
   u8 type_of_service;
   u16 total_length;
   u16 id;
   u16 fragment_offset;
   u8 time_to_live;
   u8 protocol;

   /* Two-complement additive checksum. */

   u16 checksum;
   u32 source_address;
   u32 destination_address;
   
   u8 data[0];
} __attribute__ ((packed)) ipv4_header_type;

typedef struct
{
  /* Saved first hop address. */

  u32 first_hop_address;
  u8 optlen;
  u8 srr;
  u8 rr;
  u8 ts;

  /* Set by setsockopt? */

  u8 is_setbyuser: 1;

  /* Options in __data, rather than skb. */

  u8 is_data: 1;

  /* Strict source route. */

  u8 is_strictroute: 1;

  /* Packet destination address was our one. */

  u8 srr_is_hit: 1;

  /* IP checksum more not valid. */

  u8 is_changed: 1;

  /* Need to record addr of outgoing device. */

  u8 rr_needaddr: 1;

  /* Need to record timestamp. */

  u8 ts_needtime: 1;

  /* Need to record addr of outgoing device. */

  u8 ts_needaddr: 1;
  u8 router_alert;
  u8 __pad1;
  u8 __pad2;

  /* Data start here. */

  u8 data[0];
} __attribute__ ((packed)) ipv4_options_type;




typedef struct
{
  ipv4_header_type header;
  ipv4_options_type options;

  /* Data starts here. */

  u8 data[0];
} __attribute__ ((packed)) ipv4_packet_type;




typedef struct
{
  /* The source port number. */

  u16 source_port;

  /* The destination port number. */

  u16 destination_port;

  /* The sequence number of the first data octet. */

  u32 sequence_number;

  /* If ack is set, contains the value of the next sequence number. */

  u32 acknowledgement_number;
  u16 reserved : 4;

  /* The number of 32 bit words in the TCP header. */

  u16 data_offset : 4;

  /* No more data from sender. */

  u16 finish : 1;

  /* Synchronise sequence numbers. */

  u16 synchronise : 1;

  /* Reset the connection. */

  u16 reset : 1;

  /* Push function. */

  u16 push : 1;

  /* Acknowledgement field significant. */

  u16 acknowledge : 1;

  /* Urgent pointer field signicant. */

  u16 urgent : 1;
  u16 reserved2 : 2;

  /* The number of data octets the sender is willing to accept. */
  /* Checksum of header, data and the 'pseudo header (see RFC for more
     information). */

  u16 checksum;

  /* Points to the sequence number of the octet following the urgent
     data. */

  u16 urgent_pointer;

  /* End of header; start of options. */

} __attribute__ ((packed)) tcp_header_type;




#endif
