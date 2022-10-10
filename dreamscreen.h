/* dreamscreen.h
 * Dreamscreen protocol hex values and common functions.
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

#define MAX_PACKET_LEN          18
#define MAX_PAYLOAD_LEN         MAX_PACKET_LEN - 7
#define DEFAULT_PORT            "8888"
#define DEFAULT_GROUP_ADDR      "0x00"
#define DEFAULT_STRTOK_DELIM    ","

/* Protocol documentation: https://planet.neeo.com/media/80x1kj/download/dreamscreen-v2-wifi-udp-protocol.pdf */
#define PACKET_START            0xFC
#define FLAG                    0x11
#define UPPER_CMD               0x03

#define CMD_MODE                0x01
#define MODE_SLEEP              0x00
#define MODE_VIDEO              0x01
#define MODE_MUSIC              0x02
#define MODE_AMBIENT            0x03

#define CMD_INPUT               0x20
#define INPUT_HDMI_1            0x00
#define INPUT_HDMI_2            0x01
#define INPUT_HDMI_3            0x02

#define CMD_BRIGHTNESS          0x02
#define DEFAULT_BRIGHTNESS      0x32

#define CMD_AMBIENT_MODE        0x08
#define AMBIENT_MODE_COLOR      0x01
#define AMBIENT_MODE_SCENE      0x02

#define CMD_AMBIENT_COLOR       0x05
#define DEFAULT_AMBIENT_COLOR   "\x09\x09\x09"

#define CMD_AMBIENT_SCENE       0x0D
#define AMBIENT_SCENE_RANDOM    0x00
#define AMBIENT_SCENE_FIRESIDE  0x01
#define AMBIENT_SCENE_TWINKLE   0x02
#define AMBIENT_SCENE_OCEAN     0x03
#define AMBIENT_SCENE_RAINBOW   0x04
#define AMBIENT_SCENE_JULY4TH   0x05
#define AMBIENT_SCENE_HOLYDAY   0x06
#define AMBIENT_SCENE_POP       0x07
#define AMBIENT_SCENE_FOREST    0x08

#define CMD_WIDESCREEN          0x2B
#define WIDESCREEN_AUTO         0x00
#define WIDESCREEN_ON           0x01
#define WIDESCREEN_OFF          0x02

#define CMD_SATURATION          0x06
#define CMD_FADERATE            0x0E
#define CMD_MIN_LUMINOSITY      0x0C

#define CMD_COLOR_BOOST         0x2D
#define COLOR_BOOST_OFF         0x00
#define COLOR_BOOST_LOW         0x01
#define COLOR_BOOST_MED         0x02
#define COLOR_BOOST_HIGH        0x03



struct DS_message {
  int group_addr;
  int command_upper;
  int command_lower;
  int payload[MAX_PAYLOAD_LEN];
  int payload_len;
};

unsigned char crc8(unsigned char *packet) {
    unsigned char crc_table[] =  {
        0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,0x70,0x77,0x7E,
        0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,
        0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,
        0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,
        0xE3,0xE4,0xED,0xEA,0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,
        0x9A,0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,0x57,0x50,
        0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,0x89,0x8E,0x87,0x80,0x95,
        0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,
        0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,
        0x58,0x4D,0x4A,0x43,0x44,0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,
        0x33,0x34,0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,0x3E,
        0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,0xAE,0xA9,0xA0,0xA7,
        0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,
        0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
    };
    unsigned char crc = 0x00;
    int i, size;
    size = packet[1];

    for(i = 0; i <= size; i++){
        crc = crc_table[(packet[i] ^ crc) & 0xFF];
        //printf("Position %d: 0x%02X CRC 0x%02X\n", i, packet[i], crc);
    }

    return crc;
}

void build_packet(unsigned char packet[], struct DS_message message) {
    int packet_len = message.payload_len + 5;
    memset(packet, 0x00, MAX_PACKET_LEN);

    packet[0] = PACKET_START;
    packet[1] = packet_len;
    packet[2] = message.group_addr;
    packet[3] = FLAG;
    packet[4] = message.command_upper;
    packet[5] = message.command_lower;

    for(int i = 0; i < message.payload_len && i < MAX_PAYLOAD_LEN; i++)
      packet[i + 6] = message.payload[i];

    packet[packet_len + 1] = crc8(packet);
}

struct DS_message build_message(char *group_addr, char *command, char *parameter) {
  struct DS_message message;
  message.command_upper = -1;
  message.command_lower = -1;
  message.payload[0] = -1;
  message.payload_len = -1;

  char *p;
  unsigned long ret;
  ret = strtoul(group_addr, &p, 16);

  if(ret > 0xFF)
    ret = 0xFF;

  message.group_addr = ret;

  if(strcmp(command, "mode") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_MODE;

    if(strcmp(parameter, "sleep") == 0) {
      message.payload[0] = MODE_SLEEP;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "video") == 0) {
      message.payload[0] = MODE_VIDEO;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "music") == 0) {
      message.payload[0] = MODE_MUSIC;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "ambient") == 0) {
      message.payload[0] = MODE_AMBIENT;
      message.payload_len = 1;
    }
  }

  else if(strcmp(command, "input") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_INPUT;

    if(strcmp(parameter, "1") == 0) {
      message.payload[0] = INPUT_HDMI_1;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "2") == 0) {
      message.payload[0] = INPUT_HDMI_2;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "3") == 0) {
      message.payload[0] = INPUT_HDMI_3;
      message.payload_len = 1;
    }
  }

  else if(strcmp(command, "brightness") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_BRIGHTNESS;
    ret = strtoul(parameter, &p, 10);

    if(ret > 100)
      message.payload[0] = 100;
    else
      message.payload[0] = ret;

    message.payload_len = 1;
  }

  else if(strcmp(command, "ambient_mode") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_AMBIENT_MODE;

    if(strcmp(parameter, "color") == 0) {
      message.payload[0] = AMBIENT_MODE_COLOR;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "scene") == 0) {
      message.payload[0] = AMBIENT_MODE_SCENE;
      message.payload_len = 1;
    }
  }
  
  else if(strcmp(command, "ambient_scene") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_AMBIENT_SCENE;

    if(strcmp(parameter, "random_color") == 0) {
      message.payload[0] = AMBIENT_SCENE_RANDOM;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "fireside") == 0) {
      message.payload[0] = AMBIENT_SCENE_FIRESIDE;
      message.payload_len = 1;
    }
    
    else if(strcmp(parameter, "twinkle") == 0) {
      message.payload[0] = AMBIENT_SCENE_TWINKLE;
      message.payload_len = 1;
    }
    
    else if(strcmp(parameter, "ocean") == 0) {
      message.payload[0] = AMBIENT_SCENE_OCEAN;
      message.payload_len = 1;
    }
    
    else if(strcmp(parameter, "rainbow") == 0) {
      message.payload[0] = AMBIENT_SCENE_RAINBOW;
      message.payload_len = 1;
    }
    
    else if(strcmp(parameter, "july4th") == 0) {
      message.payload[0] = AMBIENT_SCENE_JULY4TH;
      message.payload_len = 1;
    }
    
    else if(strcmp(parameter, "holyday") == 0) {
      message.payload[0] = AMBIENT_SCENE_HOLYDAY;
      message.payload_len = 1;
    }
    
    else if(strcmp(parameter, "pop") == 0) {
      message.payload[0] = AMBIENT_SCENE_POP;
      message.payload_len = 1;
    }
    
    else if(strcmp(parameter, "enchanted_forest") == 0) {
      message.payload[0] = AMBIENT_SCENE_FOREST;
      message.payload_len = 1;
    }
  }

  else if(strcmp(command, "ambient_color") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_AMBIENT_COLOR;

    char *pp;
    int i = 0;

    p = strtok(parameter, DEFAULT_STRTOK_DELIM);

    for(i = 0; i < 3 && p != NULL; i++) {
      ret = strtoul(p, &pp, 10);

      if(ret > 0xFF)
        message.payload[i] = 0xFF;
      else
        message.payload[i] = ret;

      p = strtok(NULL, DEFAULT_STRTOK_DELIM);
      message.payload_len = i + 1;
    }

    if(i != 3)
      message.payload[0] = -1;
  }

  else if(strcmp(command, "saturation") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_SATURATION;

    char *pp;
    int i = 0;

    p = strtok(parameter, DEFAULT_STRTOK_DELIM);

    for(i = 0; i < 3 && p != NULL; i++) {
      ret = strtoul(p, &pp, 10);

      if(ret > 0xFF)
        message.payload[i] = 0xFF;
      else
        message.payload[i] = ret;

      p = strtok(NULL, DEFAULT_STRTOK_DELIM);
      message.payload_len = i + 1;
    }

    if(i != 3)
      message.payload[0] = -1;
  }

  else if(strcmp(command, "widescreen") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_WIDESCREEN;

    if(strcmp(parameter, "auto") == 0) {
      message.payload[0] = WIDESCREEN_AUTO;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "on") == 0) {
      message.payload[0] = WIDESCREEN_ON;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "off") == 0) {
      message.payload[0] = WIDESCREEN_OFF;
      message.payload_len = 1;
    }
  }

  else if(strcmp(command, "fade_rate") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_FADERATE;
    ret = strtoul(parameter, &p, 10);

    if(ret > 50)
      message.payload[0] = 50;
    else
      message.payload[0] = ret;

    message.payload_len = 1;
  }

  else if(strcmp(command, "minimum_luminosity") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_MIN_LUMINOSITY;

    char *pp;
    int i = 0;

    p = strtok(parameter, DEFAULT_STRTOK_DELIM);

    for(i = 0; i < 3 && p != NULL; i++) {
      ret = strtoul(p, &pp, 10);

      if(ret > 50)
        message.payload[i] = 50;
      else
        message.payload[i] = ret;

      p = strtok(NULL, DEFAULT_STRTOK_DELIM);
      message.payload_len = i + 1;
    }

    if(i != 3)
      message.payload[0] = -1;
  }

  else if(strcmp(command, "color_boost") == 0) {
    message.command_upper = UPPER_CMD;
    message.command_lower = CMD_COLOR_BOOST;

    if(strcmp(parameter, "off") == 0) {
      message.payload[0] = COLOR_BOOST_OFF;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "low") == 0) {
      message.payload[0] = COLOR_BOOST_LOW;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "medium") == 0) {
      message.payload[0] = COLOR_BOOST_MED;
      message.payload_len = 1;
    }

    else if(strcmp(parameter, "high") == 0) {
      message.payload[0] = COLOR_BOOST_HIGH;
      message.payload_len = 1;
    }
  }


  return message;
}
