// NAME: Menus.cpp
//
#include <LCDMenuController.h>
#include "Bluetooth.h"
#include "RoboRemote_Pins.h"

extern LCD lcd;
extern LCDMenuController menu;

extern void *remoteControl();

menuFuncPtr progSlave(const LCDMenuController *) {
  //                              0123456789012345
  lcd.clear();         lcd.print(F("Insert Slave"));
  lcd.setCursor(0, 1); lcd.print(F("Press SELECT..."));
  while (!menu.isSelectButtonPressed() && !menu.isBackButtonPressed());  // wait
  if (menu.isBackButtonPressed()) return NULL;

  //                               0123456789012345
  lcd.clear();          lcd.print(F("Programming..."));
  lcd.setCursor(0,1);   lcd.print(F("HC-05 as Slave"));
  BTserial.begin(BTspeed);
  digitalWrite(BT_CMD_PIN, HIGH); // set HIGH for command mode

  String remoteAddr;
  int rc = slaveConfig(remoteAddr);
  lcd.clear();
  if (rc < 0) {  // error
    lcd.print(F("ERROR: #")); lcd.print(rc);
    lcd.setCursor(0,1); lcd.print(F("Press BACK..."));
    while (!menu.isBackButtonPressed());  // wait
    return NULL;
  }
  else {
    lcd.print(F("Success."));
    delay(2000);
  }
  digitalWrite(BT_CMD_PIN, LOW);

  //                              0123456789012345
  lcd.clear();         lcd.print(F("Insert Master"));
  lcd.setCursor(0, 1); lcd.print(F("Press SELECT..."));
  while (!menu.isSelectButtonPressed() && !menu.isBackButtonPressed());  // wait
  if (menu.isBackButtonPressed()) return NULL;
  
  //                               0123456789012345
  lcd.clear();          lcd.print(F("Programming..."));
  lcd.setCursor(0,1);   lcd.print(F("HC-05 as Master"));
  digitalWrite(BT_CMD_PIN, HIGH); // set HIGH for command mode
  
  rc = masterConfig(remoteAddr);
  lcd.clear();
  if (rc < 0) {  // error
    lcd.print(F("ERROR: #")); lcd.print(rc);
    lcd.setCursor(0,1); lcd.print(F("Press BACK..."));
    while (!menu.isBackButtonPressed());  // wait
    return NULL;
  }
  else {
    lcd.print(F("Success."));
  }
  digitalWrite(BT_CMD_PIN, LOW);
  
  lcd.setCursor(0,1); lcd.print(F("Press BACK..."));
  while (!menu.isBackButtonPressed());  // wait
  return NULL;  
}

menuFuncPtr pair(const LCDMenuController *) {
  lcd.setCursor(0, 1);
  lcd.print(F("Pairing...      "));

  // start communication with the HC-05
  BTserial.begin(BTspeed);
  digitalWrite(BT_CMD_PIN, LOW);  // switch to communication mode
  while (!digitalRead(BT_STATE_PIN)); // wait for connection
  digitalWrite(BLUE_LED_PIN, HIGH);

  lcd.setCursor(0, 1);
  lcd.print(F("Paired!         "));
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print(F("       "));

  return remoteControl();
}
