# INA219

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
