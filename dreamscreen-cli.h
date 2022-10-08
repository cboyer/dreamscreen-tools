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
  ambient_mode \t Ambient mode\n\
  \t\t [color, scene]\n\
  ambient_color\t Ambient mode color RGB, comma separated\n\
  \t\t [0-255],[0-255],[0-255]\n\
  ambient_scene\t Ambient mode scene\n\
  \t\t [random_color, fireside, twinkle, ocean, rainbow, july4th, holyday, pop, enchanted_forest]\n\
    \n\n");
}
