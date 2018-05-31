/*****************************************************************
DREAMSCREEN DAEMON - https://github.com/cboyer/dreamscreen-daemon
Cyril Boyer, https://cboyer.github.io - 2018-05-26
Licensed under GPLv3
*****************************************************************/

#include <linux/input.h>

/* key mapping, configured to work with remote-arduino, https://github.com/cboyer/remote-arduino */
#define DS_COMBINATION_KEY KEY_LEFTALT
#define DS_KEY_MODE_SLEEP KEY_1
#define DS_KEY_MODE_VIDEO KEY_2
#define DS_KEY_MODE_MUSIC KEY_3
#define DS_KEY_MODE_AMBIENT KEY_4
#define DS_KEY_INPUT_HDMI_1 KEY_5
#define DS_KEY_INPUT_HDMI_2 KEY_6
#define DS_KEY_INPUT_HDMI_3 KEY_7
#define DS_KEY_BRIGHTNESS_VALUE_UP KEY_8
#define DS_KEY_BRIGHTNESS_VALUE_DOWN KEY_9
