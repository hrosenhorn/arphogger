#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

#include "ethernet.h"
#include "ipv4.h"
#include "raw.h"

int raw_socket;
unsigned char buffer[2048];
struct sockaddr_ll saddr;

int main (int arguments, char *argument[])
{
  unsigned long int size;
  unsigned long int counter;
  fd_set sockets;
  struct timeval timeout;

  char *lines;

  unsigned long int temp;
  unsigned long int bytes = 0;

  raw_socket = raw_init (ETHNAME);

  while (1)
  {
    FD_ZERO (&sockets);
    FD_SET (raw_socket, &sockets);

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    select (raw_socket + 1, &sockets, NULL, NULL, &timeout);


    if (FD_ISSET (raw_socket, &sockets))
    {
      raw_parse (raw_socket, buffer);
    }

  }
}
