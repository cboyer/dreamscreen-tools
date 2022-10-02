/* dreamscreend.h
 * Functions used by dreamscreend.
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

#define MAX_LEN 500
#define SYS_INPUT_PATH "/sys/class/input/"
#define DEV_INPUT_PATH "/dev/input/"

int volatile keep_running = 1;

void exit_handle() {
    keep_running = 0;
}

void usage(void) {
    printf("\
Usage: -h <host> -p <port> -d <device name>\n\
Send a command to a Dreamscreen device\n\n\
  -h \t Dreamscreen's hostname or ip address\n\
  -p \t Dreamscreen's port, if not provided the default value is 8888\n\
  -d \t Device name for event listening\
    \n\n");
}
