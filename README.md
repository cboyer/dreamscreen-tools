# dreamscreen-daemon
Remotely control [Dreamscreen device](https://www.dreamscreentv.com) with [Remote-arduino](https://github.com/cboyer/remote-arduino) or any keyboard-like device from Linux based media centers instead of using a cellphone and official application.

Currently, only modes/inputs/brightness are implemented.

Use a key combination in order to avoid unwanted interaction with Kodi.

Default mapping:

Name | Key | Command
--- | --- | ---
DS_COMBINATION_KEY | Left ALT | Key used for combination
DS_KEY_MODE_SLEEP | 1 | Sleep mode, no light
DS_KEY_MODE_VIDEO | 2 | Mode video
DS_KEY_MODE_MUSIC | 3 | Mode music
DS_KEY_MODE_AMBIENT | 4 | Mode ambient
DS_KEY_INPUT_HDMI_1 | 5 | Signal source: HDMI 1
DS_KEY_INPUT_HDMI_2 | 6 | Signal source: HDMI 2
DS_KEY_INPUT_HDMI_3 | 7 | Signal source: HDMI 3
DS_KEY_BRIGHTNESS_VALUE_UP | 8 | Set brightness + 10%
DS_KEY_BRIGHTNESS_VALUE_DOWN | 9 | Set brightness - 10%

To change keys, use Linux input event codes: https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h

## Prerequisites

Your system account must be in the *input* group to open /dev/input/eventX or you will get *ERROR: cannot open /dev/input/eventX: Permission denied.*

```bash
sudo usermod -a -G input myaccount
```

Find the correct */dev/input/event* for your device:
```bash
cat /dev/input/eventX | hexdump
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

## Installation

Edit *dreamscreen.service* to your needs (user account, eventX).
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
dreamscreend 192.168.1.102 8888 /dev/input/event2
```

Start dreamscreen-daemon with systemd:
```bash
sudo systemctl start dreamscreen
```

enable dreamscreen at boot:
```bash
sudo systemctl enable dreamscreen
```

stop dreamscreen daemon with systemd:
```bash
sudo systemctl stop dreamscreen
```

## Uninstall

Uninstall dreamscreen-daemon:
```bash
sudo make uninstall
```
