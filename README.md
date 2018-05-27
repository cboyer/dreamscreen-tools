# dreamscreen-daemon
Remotely control Dreamscreen device with [Remote-arduino](https://github.com/cboyer/remote-arduino) or any keyboard-like device from Linux based systems instead of using a cellphone and official application.

Currently, only modes/inputs/brightness are implemented.

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
