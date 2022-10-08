Tools to control a Dreamscreen device from Linux.

Dreamscreen protocol documentation is available [here](https://planet.neeo.com/media/80x1kj/download/dreamscreen-v2-wifi-udp-protocol.pdf).

## dreamscreen-cli
Simple command line utility to send command.

Usage: dreamscreen-cli -h [host or ip] -p [port] [COMMAND] [PARAMETER]

Command         | Parameter
---             |---
mode            | sleep, video, music, ambient
input           | 1, 2, 3
brightness      | integer value from 0 to 100
ambient_mode    | color, scene
ambient_color   | [0-255],[0-255],[0-255]
ambient_scene   | random_color, fireside, twinkle, ocean, rainbow, july4th, holyday, pop, enchanted_forest

## dreamscreend
Controls Dreamscreen with keyboard event on Linux.

Usage: dreamscreend -h [host or ip] -p [port] -d [device name]

A key combination is used in order to avoid unwanted interaction with Kodi and other application.

Edit *dreamscreend_keys.h* to configure keys to your needs with [Linux input event codes](https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h).

Name | Key | Command
--- | --- | ---
KEY_COMBINATION | Left ALT | Key used for combination
KEY_MODE_SLEEP | 1 | Sleep mode, no light
KEY_MODE_VIDEO | 2 | Video mode
KEY_MODE_MUSIC | 3 | Music mode
KEY_MODE_AMBIENT | 4 | Ambient mode
KEY_INPUT_HDMI_1 | 5 | Signal source: HDMI 1
KEY_INPUT_HDMI_2 | 6 | Signal source: HDMI 2
KEY_INPUT_HDMI_3 | 7 | Signal source: HDMI 3
KEY_BRIGHTNESS_VALUE_UP | 8 | Set brightness + 10%
KEY_BRIGHTNESS_VALUE_DOWN | 9 | Set brightness - 10%

For example to activate video mode, use: *Left ALT + 2*


## Prerequisites
User account should be in the *input* group to open /dev/input/eventX and to avoid *ERROR: cannot open /dev/input/eventX: Permission denied.*

```bash
sudo usermod -a -G input kodi
```

## Compilation
Clone this repository and code compilation:
```bash
git clone https://github.com/cboyer/dreamscreen-tools
cd dreamscreen-tools
make all
```

Alternatively, you can compile code with `make debug` to print key events and UDP packet content.
Compile only one tool with `make cli` for dreamscreen-cli and `make dreamscreend` for dreamscreend.

## Installation
Edit *dreamscreend.service* to your needs (user account, device name).
```bash
vi dreamscreend.service
sudo make install

sudo systemctl start dreamscreend
sudo systemctl enable dreamscreend
```

## Uninstall
Uninstall dreamscreen-daemon:
```bash
sudo make uninstall
```
