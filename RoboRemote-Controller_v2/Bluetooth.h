// NAME: Bluetooth.h
//
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

#define BTserial  Serial
#define BTspeed   38400

int slaveConfig(String& remoteAddr);
int masterConfig(const String& remoteAddr);

#endif
