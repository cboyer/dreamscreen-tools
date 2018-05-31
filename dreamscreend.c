/*****************************************************************
DREAMSCREEN DAEMON - https://github.com/cboyer/dreamscreen-daemon
Cyril Boyer, https://cboyer.github.io - 2018-05-26
Licensed under GPLv3
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include "key_mapping.h"


/* CRC8 calculation */
unsigned char calcCRC8(unsigned char *packet) {
  unsigned char crcTable[] =
  {0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3};
  unsigned char crc = 0x00;
  int i, size;
  size = packet[1];

  for(i = 0; i <= size; i++){
    crc = crcTable[(packet[i] ^ crc) & 0xFF];
    //printf("Position %d: 0x%02X CRC 0x%02X\n", i, packet[i], crc);
  }
  return crc;
}

/* packet assemblage */
void assemble_packet(unsigned char packet[], unsigned char prefix[], unsigned char upper_command, unsigned char lower_command){
  memcpy(packet, prefix, 5);
  packet[5] = upper_command;
  packet[6] = lower_command;
  packet[7] = calcCRC8(packet);
}


int main(int argc, char **argv) {

  #ifdef DEBUG
    static const char *const evval[] = { "released", "pressed", "repeated" };
    int i;
  #endif

  char *dev;
  struct input_event ev;
  ssize_t n;
  int fd, sockfd, portno, p;
  struct sockaddr_in serveraddr;
  struct hostent *server;
  char *hostname;
  unsigned char packet[8];
  _Bool combination = 0;

  /* hex codes used by Dreamscreen: https://planet.neeo.com/media/80x1kj/download/dreamscreen-v2-wifi-udp-protocol.pdf */
  unsigned char prefix[] = { 0xFC, 0x06, 0x00, 0x11, 0x03 };
  unsigned char mode = 0x01;
  unsigned char mode_sleep = 0x00;
  unsigned char mode_video = 0x01;
  unsigned char mode_music = 0x02;
  unsigned char mode_ambient = 0x03;
  unsigned char input = 0x20;
  unsigned char input_hdmi_1 = 0x00;
  unsigned char input_hdmi_2 = 0x01;
  unsigned char input_hdmi_3 = 0x02;
  unsigned char brightness = 0x02;
  unsigned char brightness_value = 0x0A;

  /* check command line arguments */
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <hostname> <port> <input>\n", argv[0]);
    fprintf(stderr, "Example: %s 192.168.0.22 8888 /dev/input/event3\n", argv[0]);
    fprintf(stderr, "Tips: find working input with: cat /dev/input/eventX | hexdump\n\n");
    exit(EXIT_FAILURE);
  }
  hostname = argv[1];
  portno = atoi(argv[2]);
  dev = argv[3];

  /* open keyboard input */
  fd = open(dev, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "ERROR: cannot open %s: %s.\n", dev, strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* create socket */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("ERROR: cannot open socket");
    exit(EXIT_FAILURE);
  }

  /* get Dreamscreen DNS entry */
  server = gethostbyname(hostname);
  if (server == NULL) {
    fprintf(stderr,"ERROR: no such host as %s\n", hostname);
    exit(EXIT_FAILURE);
  }

  /* build Dreamscreen address */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(portno);

  /* main loop, read keyboard input */
  while (1) {
    n = read(fd, &ev, sizeof ev);
    if (n == (ssize_t)-1) {
      if (errno == EINTR)
        continue;
      else
        break;
    }
    else
    if (n != sizeof ev) {
      errno = EIO;
      break;
    }

    bzero(packet, sizeof(packet));

    /* build packet */
    if (ev.type == EV_KEY && ev.value >= 0 && ev.value < 2) {

      /* print pressed key */
      #ifdef DEBUG
        printf("Key:\t%s 0x%02X (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
      #endif

      if (ev.code == DS_COMBINATION_KEY && ev.value == 1)
        combination = 1;

      if (ev.code == DS_COMBINATION_KEY && ev.value == 0)
        combination = 0;

      if (combination && ev.value == 1) {
        switch(ev.code) {
          case DS_KEY_MODE_SLEEP:
            assemble_packet(packet, prefix, mode, mode_sleep);
            break;

          case DS_KEY_MODE_VIDEO:
            assemble_packet(packet, prefix, mode, mode_video);
            break;

          case DS_KEY_MODE_MUSIC:
            assemble_packet(packet, prefix, mode, mode_music);
            break;

          case DS_KEY_MODE_AMBIENT:
            assemble_packet(packet, prefix, mode, mode_ambient);
            break;

          case DS_KEY_INPUT_HDMI_1:
            assemble_packet(packet, prefix, input, input_hdmi_1);
            break;

          case DS_KEY_INPUT_HDMI_2:
            assemble_packet(packet, prefix, input, input_hdmi_2);
            break;

          case DS_KEY_INPUT_HDMI_3:
            assemble_packet(packet, prefix, input, input_hdmi_3);
            break;

          case DS_KEY_BRIGHTNESS_VALUE_UP:
            brightness_value = brightness_value + 10;
            if (brightness_value > 100)
              brightness_value = 10;

            assemble_packet(packet, prefix, brightness, brightness_value);
            break;

          case DS_KEY_BRIGHTNESS_VALUE_DOWN:
            if (brightness_value >= 20)
              brightness_value = brightness_value - 10;
            else
              brightness_value = 100;

            assemble_packet(packet, prefix, brightness, brightness_value);
            break;

          default:
            bzero(packet, sizeof(packet));
        }
      }

      /* send packet to Dreamscreen */
      if (packet[0] == 0xFC) {

        #ifdef DEBUG
          /* print packet */
          printf("Packet:\t");
          for (i = 0; i < sizeof(packet); i++){
            printf("0x%02X ", packet[i]);
          }
          printf("\n");
        #endif

        p = sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
        if (p < 0)
          perror("ERROR: in sendto");
      }
    }
  }

  fflush(stdout);
  fprintf(stderr, "ERROR: %s.\n", strerror(errno));
  close(sockfd);
  close(fd);

  return 0;
}
