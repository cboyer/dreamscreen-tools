/* dreamscreen-cli.h
 * Functions used by dreamscreen-cli.
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

#define UNKNOWN 0xFF

void usage(void) {
    printf("\
Usage: -h <host> -p <port> [COMMAND] [PARAMETER]\n\
Send a command to a Dreamscreen device\n\n\
  -h \t Device's hostname or ip address\n\
  -p \t Device's port, if not provided the default value is 8888\n\n\
Available commands and parameters:\n\
  mode \t\t LEDs mode\n\
  \t\t [sleep, video, music, ambient]\n\
  input \t Select HDMI input\n\
  \t\t [1, 2, 3]\n\
  brightness \t LEDs brightness level\n\
  \t\t [0-100]\n\
    \n\n");
}

unsigned int command_hex(char *command) {
  if(strcmp(command, "mode") == 0)
    return CMD_MODE;

  else if(strcmp(command, "input") == 0)
    return CMD_INPUT;

  else if(strcmp(command, "brightness") == 0)
    return CMD_BRIGHTNESS;

  return UNKNOWN;
}

unsigned int parameter_hex(char *command, char *parameter) {
  if(strcmp(command, "mode") == 0) {

    if(strcmp(parameter, "sleep") == 0)
      return MODE_SLEEP;

    else if(strcmp(parameter, "video") == 0)
      return MODE_VIDEO;

    else if(strcmp(parameter, "music") == 0)
      return MODE_MUSIC;

    else if(strcmp(parameter, "ambient") == 0)
      return MODE_AMBIENT;
  }
  else if(strcmp(command, "input") == 0) {
    if(strcmp(parameter, "1") == 0)
      return INPUT_HDMI_1;

    else if(strcmp(parameter, "2") == 0)
      return INPUT_HDMI_2;

    else if(strcmp(parameter, "3") == 0)
      return INPUT_HDMI_3;

  }
  else if(strcmp(command, "brightness") == 0) {
    char *ptr;
    unsigned long ret;
    ret = strtoul(parameter, &ptr, 10);

    if(ret > 100)
      return 100;

    if(ret > 0)
      return ret;
  }

  return UNKNOWN;
}
