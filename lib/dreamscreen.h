/* dreamscreen.h
 * Dreamscreen protocol common functions.
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

#include "dreamscreen_hex.h"

#define DEFAULT_STRTOK_DELIM ","

struct DS_message {
  int group_addr;
  int command_upper;
  int command_lower;
  int payload[MAX_PAYLOAD_LEN];
  int payload_len;
};

struct Args {
    char command[30];
    unsigned char command_hex;
    char parameter[30];
    unsigned char parameter_hex;
};

struct Args hex_index[] = {
  { "mode", CMD_MODE, "sleep", MODE_SLEEP },
  { "mode", CMD_MODE, "video", MODE_VIDEO },
  { "mode", CMD_MODE, "music", MODE_MUSIC },
  { "mode", CMD_MODE, "ambient", MODE_AMBIENT },
  { "input", CMD_INPUT, "1", INPUT_HDMI_1 },
  { "input", CMD_INPUT, "2", INPUT_HDMI_2 },
  { "input", CMD_INPUT, "3", INPUT_HDMI_3 },
  { "brightness", CMD_BRIGHTNESS, {'\0'}, 0 },
  { "ambient_mode", CMD_AMBIENT_MODE, "color", AMBIENT_MODE_COLOR },
  { "ambient_mode", CMD_AMBIENT_MODE, "scene", AMBIENT_MODE_SCENE },
  { "ambient_scene", CMD_AMBIENT_SCENE, "random_color", AMBIENT_SCENE_RANDOM },
  { "ambient_scene", CMD_AMBIENT_SCENE, "fireside", AMBIENT_SCENE_FIRESIDE },
  { "ambient_scene", CMD_AMBIENT_SCENE, "twinkle", AMBIENT_SCENE_TWINKLE },
  { "ambient_scene", CMD_AMBIENT_SCENE, "ocean", AMBIENT_SCENE_OCEAN },
  { "ambient_scene", CMD_AMBIENT_SCENE, "rainbow", AMBIENT_SCENE_RAINBOW },
  { "ambient_scene", CMD_AMBIENT_SCENE, "july4th", AMBIENT_SCENE_JULY4TH },
  { "ambient_scene", CMD_AMBIENT_SCENE, "holyday", AMBIENT_SCENE_HOLYDAY },
  { "ambient_scene", CMD_AMBIENT_SCENE, "pop", AMBIENT_SCENE_POP },
  { "ambient_scene", CMD_AMBIENT_SCENE, "enchanted_forest", AMBIENT_SCENE_FOREST },
  { "ambient_color", CMD_AMBIENT_COLOR, {'\0'}, 0 },
  { "saturation", CMD_SATURATION, {'\0'}, 0 },
  { "widescreen", CMD_WIDESCREEN, "auto", WIDESCREEN_AUTO },
  { "widescreen", CMD_WIDESCREEN, "on", WIDESCREEN_ON },
  { "widescreen", CMD_WIDESCREEN, "off", WIDESCREEN_OFF },
  { "fade_rate", CMD_FADERATE, {'\0'}, 0 },
  { "minimum_luminosity", CMD_MIN_LUMINOSITY, {'\0'}, 0 },
  { "color_boost", CMD_COLOR_BOOST, "off", COLOR_BOOST_OFF },
  { "color_boost", CMD_COLOR_BOOST, "low", COLOR_BOOST_LOW },
  { "color_boost", CMD_COLOR_BOOST, "medium", COLOR_BOOST_MED },
  { "color_boost", CMD_COLOR_BOOST, "high", COLOR_BOOST_HIGH },
  { "music_visualizer", CMD_MUSIC_VISUALIZER, "1", MUSIC_VISUALIZER_1 },
  { "music_visualizer", CMD_MUSIC_VISUALIZER, "2", MUSIC_VISUALIZER_2 },
  { "music_visualizer", CMD_MUSIC_VISUALIZER, "3", MUSIC_VISUALIZER_3 },
  { "music_visualizer", CMD_MUSIC_VISUALIZER, "4", MUSIC_VISUALIZER_4 },
  { "music_color", CMD_MUSIC_COLOR, {'\0'}, 0 },
  { "music_brightness", CMD_MUSIC_BRIGHTNESS, {'\0'}, 0 },
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

void set_commands(struct DS_message *message, unsigned char upper_command, unsigned char lower_command) {
  message->command_upper = upper_command;
  message->command_lower = lower_command;
}

void add_payload_uchar(struct DS_message *message, unsigned char payload) {
    if(message->payload_len < 0) {
      message->payload[0] = payload;
      message->payload_len = 1;
    }
    else {
      message->payload[message->payload_len] = payload;
      message->payload_len++;
    }
}

void reset_payload(struct DS_message *message) {
  message->payload[0] = -1;
  message->payload_len = -1;
}

/*
void set_payload_arr(struct DS_message *message, char *payload, int length) {
    memcpy(message->payload, payload, length);
    message->payload_len = length;
}
*/

struct DS_message build_message(char *group_addr, char *command, char *parameter) {
  struct DS_message message;
  message.command_upper = -1;
  message.command_lower = -1;
  reset_payload(&message);

  char *p;
  unsigned long ret;
  ret = strtoul(group_addr, &p, 16);

  if(ret > 0xFF)
    ret = 0xFF;

  message.group_addr = ret;

  for(size_t i = 0; i < sizeof hex_index / sizeof hex_index[0]; i++) {
    if(strcmp(hex_index[i].command, command) == 0)
      set_commands(&message, UPPER_CMD, hex_index[i].command_hex);

    if(strcmp(hex_index[i].command, command) == 0 && strcmp(hex_index[i].parameter, parameter) == 0) {
      add_payload_uchar(&message, hex_index[i].parameter_hex);
      return message;
    }
  }

  if(strcmp(command, "brightness") == 0) {
    ret = strtoul(parameter, &p, 10);

    if(ret > 100)
      ret = 100;

    add_payload_uchar(&message, ret);
  }

  else if(strcmp(command, "ambient_color") == 0) {
    char *pp;
    p = strtok(parameter, DEFAULT_STRTOK_DELIM);

    for(int i = 0; i < 3 && p != NULL; i++) {
      ret = strtoul(p, &pp, 10);

      if(ret > 0xFF)
        ret = 0xFF;

      add_payload_uchar(&message, ret);
      p = strtok(NULL, DEFAULT_STRTOK_DELIM);
    }

    if(message.payload_len != 3)
      reset_payload(&message);
  }

  else if(strcmp(command, "saturation") == 0) {
    char *pp;
    p = strtok(parameter, DEFAULT_STRTOK_DELIM);

    for(int i = 0; i < 3 && p != NULL; i++) {
      ret = strtoul(p, &pp, 10);

      if(ret > 0xFF)
        ret = 0xFF;

      add_payload_uchar(&message, ret);
      p = strtok(NULL, DEFAULT_STRTOK_DELIM);
    }

    if(message.payload_len != 3)
      reset_payload(&message);
  }

  else if(strcmp(command, "fade_rate") == 0) {
    ret = strtoul(parameter, &p, 10);

    if(ret > 50)
      ret = 50;

    add_payload_uchar(&message, ret);
  }

  else if(strcmp(command, "minimum_luminosity") == 0) {
    char *pp;
    p = strtok(parameter, DEFAULT_STRTOK_DELIM);

    for(int i = 0; i < 3 && p != NULL; i++) {
      ret = strtoul(p, &pp, 10);

      if(ret > 50)
        ret = 50;

      add_payload_uchar(&message, ret);
      p = strtok(NULL, DEFAULT_STRTOK_DELIM);
      message.payload_len = i + 1;
    }

    if(message.payload_len != 3)
      reset_payload(&message);
  }

  else if(strcmp(command, "music_color") == 0) {
    p = strtok(parameter, DEFAULT_STRTOK_DELIM);

    for(int i = 0; i < 3 && p != NULL; i++) {

      if(p[0] == 'r')
        add_payload_uchar(&message, MUSIC_COLOR_RED);

      else if(p[0] == 'g')
        add_payload_uchar(&message, MUSIC_COLOR_GREEN);

      else if(p[0] == 'b')
        add_payload_uchar(&message, MUSIC_COLOR_BLUE);

      p = strtok(NULL, DEFAULT_STRTOK_DELIM);
    }

    if(message.payload_len != 3)
      reset_payload(&message);
  }

  else if(strcmp(command, "music_brightness") == 0) {
    char *pp;
    p = strtok(parameter, DEFAULT_STRTOK_DELIM);

    for(int i = 0; i < 3 && p != NULL; i++) {
      ret = strtoul(p, &pp, 10);

      if(ret > 100)
        ret = 100;

      add_payload_uchar(&message, ret);
      p = strtok(NULL, DEFAULT_STRTOK_DELIM);
    }

    if(message.payload_len != 3)
      reset_payload(&message);
  }

  return message;
}
