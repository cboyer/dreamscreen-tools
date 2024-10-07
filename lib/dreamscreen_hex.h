/* dreamscreen_hex.h
 * Dreamscreen protocol hex values.
 * Protocol documentation: https://community.ezlo.com/uploads/default/original/3X/f/c/fc0bccb95282ad1bc7c4a4236740a840641a6de4.pdf
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

#define CMD_MUSIC_VISUALIZER    0x09
#define MUSIC_VISUALIZER_1      0x00
#define MUSIC_VISUALIZER_2      0x01
#define MUSIC_VISUALIZER_3      0x02
#define MUSIC_VISUALIZER_4      0x03

#define CMD_MUSIC_COLOR         0x0A
#define MUSIC_COLOR_RED         0x00
#define MUSIC_COLOR_GREEN       0x01
#define MUSIC_COLOR_BLUE        0x02

#define CMD_MUSIC_BRIGHTNESS    0x0B
