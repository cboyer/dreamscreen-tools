# Dreamscreen-daemon
Control [Dreamscreen device](https://www.dreamscreentv.com) with keyboard event on Linux.

Currently, only modes/inputs/brightness are implemented.
Dreamscreen protocol documentation is available [here](https://planet.neeo.com/media/80x1kj/download/dreamscreen-v2-wifi-udp-protocol.pdf).


## Default key mapping
A key combination is used in order to avoid unwanted interaction with Kodi and other application.

Edit *key_mapping.h* to configure keys to your needs with [Linux input event codes](https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h).

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
sudo usermod -a -G input myaccount
```

## Compilation
Clone this repository:
```bash
git clone https://github.com/cboyer/dreamscreen-daemon
```

Compile source code:
```bash
cd dreamscreen-daemon
make
```

Compile source code with debug option (print key events and UDP packet content):
```bash
cd dreamscreen-daemon
make debug
```

## Installation
Edit *dreamscreen.service* to your needs (user account, device name).
```bash
vi dreamscreen.service
```

Install dreamscreen-daemon:
```bash
sudo make install
```

## Run dreamscreen-daemon
You can run the binary manually:
```bash
dreamscreend -h 192.168.1.102 -p 8888 -d "Arduino LLC Arduino Leonardo"
```

Start dreamscreen-daemon with systemd:
```bash
sudo systemctl start dreamscreen
```

Enable dreamscreen-daemon at boot:
```bash
sudo systemctl enable dreamscreen
```

Stop dreamscreen-daemon with systemd:
```bash
sudo systemctl stop dreamscreen
```

## Uninstall
Uninstall dreamscreen-daemon:
```bash
sudo make uninstall
```
