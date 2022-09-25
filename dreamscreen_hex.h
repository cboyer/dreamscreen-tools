/* dreamscreen_hex.h
 * Dreamscreen protocol hex values.
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

/* Protocol documentation: https://planet.neeo.com/media/80x1kj/download/dreamscreen-v2-wifi-udp-protocol.pdf */
#define PACKET_START        0xFC
#define FIXED_LENGTH        0x06
#define GROUP_ADDR          0x00
#define FLAG                0x11
#define UPPER_CMD           0x03

#define CMD_MODE            0x01
#define MODE_SLEEP          0x00
#define MODE_VIDEO          0x01
#define MODE_MUSIC          0x02
#define MODE_AMBIENT        0x03

#define CMD_INPUT           0x20
#define INPUT_HDMI_1        0x00
#define INPUT_HDMI_2        0x01
#define INPUT_HDMI_3        0x02

#define CMD_BRIGHTNESS      0x02
#define DEFAULT_BRIGHTNESS  0x0A
