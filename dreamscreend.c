/* dreamscreend.c
 * Control Dreamscreen device with keyboard event on Linux.
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
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <getopt.h>
#include "dreamscreen.h"
#include "key_mapping.h"


int main(int argc, char **argv) {
  int opt, fd, sock, combination = 0, p;
  ssize_t n;
  DIR *dp;
  FILE *file;
  unsigned char packet[8];
  char buf[MAX_LEN], device_path[MAX_LEN];
  char *host = NULL, *port = NULL, *device_name = NULL;
  struct dirent *ep;
  struct sigaction act;
  struct hostent *dest;
  struct sockaddr_in dest_addr;
  struct input_event ev;

  unsigned char brightness_value = DEFAULT_BRIGHTNESS;

  #ifdef DEBUG
  static const char *const evval[] = { "released", "pressed", "repeated" };
  #endif

  while((opt = getopt(argc, argv, ":h:p:d:")) != -1) {
    switch(opt) { 
      case 'h':
        host = optarg;
        break;

      case 'p':
        port = optarg;
        break;

      case 'd':
        device_name = optarg;
        break;

      case ':':
        printf("Option needs a value\n");
        show_usage(argv[0]);
        return -1;

      case '?':
        printf("Unknown option: %c\n", optopt);
        show_usage(argv[0]);
        return -1;
    }
  }

  if(host == NULL || port == NULL || device_name == NULL) {
    fprintf(stderr, "Missing parameters\n");
    show_usage(argv[0]);
    return -1;
  }

  dp = opendir(SYS_INPUT_PATH);
  if(dp == NULL) {
    perror("Couldn't open the directory");
    return -1;
  }

  /* Find correct /dev/input with device name in /sys/class/input/eventXX/device/name */
  while((ep = readdir(dp)) != NULL) {
    if(strstr(ep->d_name, "event")) {
      strcpy(buf, SYS_INPUT_PATH);
      strcat(buf, ep->d_name);
      strcat(buf, "/device/name");
      strcpy(device_path, DEV_INPUT_PATH);
      strcat(device_path, ep->d_name);

      file = fopen(buf, "r");
      if(file == NULL) {
          perror("Couldn't open the file");
          return -1;
      }

      while(fgets(buf, sizeof(buf), file) != NULL);
      buf[strcspn(buf, "\n")] = 0;

      if(!strcmp(buf, device_name))
        break;

      fclose(file);
      device_path[0] = '\0';
    }
  }

  closedir(dp);
  if(!strcmp(device_path, "")) {
    printf("Device '%s' not found.\n", device_name);
    return -1;
  }

  /* Open device */
  printf("Device '%s' at %s\n", device_name, device_path);
  fd = open(device_path, O_RDONLY);
  if(fd == -1) {
    fprintf(stderr, "ERROR: cannot open %s: %s.\n", device_path, strerror(errno));
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
  bzero((char *) &dest_addr, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  bcopy((char *)dest->h_addr,
  (char *)&dest_addr.sin_addr.s_addr, dest->h_length);
  dest_addr.sin_port = htons(atoi(port));

  /* Register signal to stop main loop */
  act.sa_handler = exit_handle;
  sigemptyset (&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGINT,  &act, 0);
  sigaction(SIGTERM, &act, 0);

  /* Main loop: read keyboard events */
  while(keep_running) {
    n = read(fd, &ev, sizeof(ev));
    if(n == (ssize_t)-1) {
      if(errno == EINTR)
        continue;
      else
        break;
    }
    else if(n != sizeof(ev)) {
      errno = EIO;
      break;
    }

    bzero(packet, sizeof(packet));

    /* Build UDP packet from keyboard event */
    if(ev.type == EV_KEY && ev.value >= 0 && ev.value < 2) {

      #ifdef DEBUG
        printf("Key:\t%s 0x%02X (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
      #endif

      if(ev.code == KEY_COMBINATION && ev.value == 1)
        combination = 1;

      if(ev.code == KEY_COMBINATION && ev.value == 0)
        combination = 0;

      if(combination && ev.value == 1) {
        switch(ev.code) {
          case KEY_MODE_SLEEP:
            build_packet(packet, CMD_MODE, MODE_SLEEP);
            break;

          case KEY_MODE_VIDEO:
            build_packet(packet, CMD_MODE, MODE_VIDEO);
            break;

          case KEY_MODE_MUSIC:
            build_packet(packet, CMD_MODE, MODE_MUSIC);
            break;

          case KEY_MODE_AMBIENT:
            build_packet(packet, CMD_MODE, MODE_AMBIENT);
            break;

          case KEY_INPUT_HDMI_1:
            build_packet(packet, CMD_INPUT, INPUT_HDMI_1);
            break;

          case KEY_INPUT_HDMI_2:
            build_packet(packet, CMD_INPUT, INPUT_HDMI_2);
            break;

          case KEY_INPUT_HDMI_3:
            build_packet(packet, CMD_INPUT, INPUT_HDMI_3);
            break;

          case KEY_BRIGHTNESS_VALUE_UP:
            brightness_value = brightness_value + 10;
            if(brightness_value > 100)
              brightness_value = 10;

            build_packet(packet, CMD_BRIGHTNESS, brightness_value);
            break;

          case KEY_BRIGHTNESS_VALUE_DOWN:
            if(brightness_value >= 20)
              brightness_value = brightness_value - 10;
            else
              brightness_value = 100;

            build_packet(packet, CMD_BRIGHTNESS, brightness_value);
            break;

          default:
            bzero(packet, sizeof(packet));
        }
      }

      /* Send packet to Dreamscreen */
      if(packet[0] == PACKET_START) {

        #ifdef DEBUG
          printf("Packet:\t");
          for (int i = 0; i < sizeof(packet); i++) {
            printf("0x%02X ", packet[i]);
          }
          printf("\n");
        #endif

        p = sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
        if(p < 0)
          perror("ERROR: in sendto");
      }
    }
  }

  printf("Exiting...\n");
  fflush(stdout);
  close(sock);
  close(fd);
  return 0;
}
