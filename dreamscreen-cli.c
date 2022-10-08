/* dreamscreen-cli.c
 * Control Dreamscreen device from command line.
 * Copyright (C) 2022 C. Boyer
 *
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include "dreamscreen.h"
#include "dreamscreen-cli.h"


int main(int argc, char **argv) {
  int opt, sock;
  unsigned char packet[MAX_PACKET_LEN];
  char *host = NULL, *port = NULL, *command = NULL, *parameter = NULL;
  struct hostent *dest;
  struct sockaddr_in dest_addr;
  struct DS_message message;

  while((opt = getopt(argc, argv, ":p:h:")) != -1) {
    switch(opt) { 
      case 'h':
        host = optarg;
        break;

      case 'p':
        port = optarg;
        break;

      case ':':
        printf("Option needs a value\n");
        usage();
        return -1;

      case '?':
        printf("Unknown option: %c\n", optopt);
        usage();
        return -1;
    }
  }

  for(int i = optind; i < argc; i++)
    switch(i - optind){
      case 0:
        command = argv[i];
        break;
      case 1:
        parameter = argv[i];
        break;
    }

  if(port == NULL)
    port = DEFAULT_PORT;

  if(host == NULL || command == NULL || parameter == NULL) {
    fprintf(stderr, "Missing parameters\n");
    usage();
    return -1;
  }

  message = build_message(command, parameter);

  if(message.command_lower == -1) {
    fprintf(stderr, "Unknown command '%s'\n", command);
    usage();
    return -1;
  }

  if(message.payload[0] == -1) {
    fprintf(stderr, "Unknown parameter '%s' for command '%s'\n", parameter, command);
    usage();
    return -1;
  }

  /* Create socket */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) {
    perror("ERROR: cannot open socket");
    return -1;
  }

  /* Resolve DNS entry */
  dest = gethostbyname(host);
  if(dest == NULL) {
    fprintf(stderr,"ERROR: no such host %s\n", host);
    return -1;
  }

  /* Build destination address */
  memset(&dest_addr, 0x00, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  bcopy((char *)dest->h_addr,
  (char *)&dest_addr.sin_addr.s_addr, dest->h_length);
  dest_addr.sin_port = htons(atoi(port));

  /* Build and send packet to Dreamscreen */
  build_packet(packet, message);

  #ifdef DEBUG
    printf("Packet:\t");
    for (int i = 0; i < packet[1] + 2; i++) {
      printf("0x%02X ", packet[i]);
    }
    printf("\n");
  #endif

  if(sendto(sock, packet, packet[1] + 2, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr)) < 0)
    perror("ERROR: in sendto");

  fflush(stdout);
  close(sock);
  return 0;
}
