// NAME: Bluetooth.h
//
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

#define BTserial  Serial
#define BTspeed   38400

int BTconnect();
String sendCommand(const char *cmd);
String getStatus(const String& response);
String getAnswer(const String& response);

#endif
