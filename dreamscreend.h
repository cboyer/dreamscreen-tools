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

void show_usage(char *exec_name) {
    fprintf(stderr, "Usage: %s -h <host> -p <port> -d <device name>\n", exec_name);
    fprintf(stderr, "Example: %s -h 192.168.0.22 -p 8888 -d \"my_keyboard\"\n", exec_name);
}
