# INA219

In this repository I hookup an Adafruit INA219 breakout board and a Sparkfun ESP8266 Thing together to log current readings from another project. Here I use the [ArduinoINA219](https://github.com/johngineer/ArduinoINA219) library intead of adafruit's since it has less dependencies and seamed to me easier to use in my quick tests. The data logging to a google spreadsheet is based on this [youtube tutorial from Uteh Str](https://youtu.be/okNECYf2xlY), but somehow I wasn't able to unrar the file in the video description( maybe OS doesn't like rar stuff?? ) so I typed what I could read in the video and made some changes wherever needed. Seems to be working fine.

## Before Compiling

Create a file ```./secrets/secrets.h``` before trying to compile, as follow:

```c
#ifndef SECRETS_SECRETS_H
#define SECRETS_SECRETS_H

const char* ssid = "my_ssid"; // Your WiFi SSID
const char* password = "my_wifi_password"; // Your WiFi Password

const char *GScriptId = ""; //Google Script ID

#endif //SECRETS_SECRETS_H
```

## Flash Command

Apparently, there is some bug in between my arduino IDE and my python3 that prevents esptools to flash esp8266 correctly in version 2.7.4 of board library. Thus I'm using the following command to flash the firmware from the project folder.

```bash
python ~/.arduino15/packages/esp8266/hardware/esp8266/2.7.4/tools/esptool/esptool.py --baud=115200 --port /dev/ttyUSB0 --chip esp8266 -a soft_reset write_flash 0x00000 ./build/corrente.ino.bin
```
