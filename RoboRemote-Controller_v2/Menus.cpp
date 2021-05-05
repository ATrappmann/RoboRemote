// NAME: Menus.cpp
//
#include <LCDMenuController.h>
#include "Bluetooth.h"
#include "RoboRemote_Pins.h"

extern LCD lcd;
extern LCDMenuController menu;

extern void *remoteControl();

menuFuncPtr progSlave(const LCDMenuController *) {
  //                             0123456789012345
  lcd.clear();         lcd.print("HC-05 as Slave");
  lcd.setCursor(0, 1); lcd.print("Press SELECT..");
  while (!menu.isSelectButtonPressed() && !menu.isBackButtonPressed());  // wait
  if (menu.isBackButtonPressed()) return NULL;
  
  //                             0123456789012345
  lcd.clear();         lcd.print("Insert Slave");
  lcd.setCursor(0, 1); lcd.print("Press SELECT..");
  while (!menu.isSelectButtonPressed() && !menu.isBackButtonPressed());  // wait
  if (menu.isBackButtonPressed()) return NULL;

  //                               0123456789012345
  lcd.clear();          lcd.print("Programming");
  lcd.setCursor(0,1);   lcd.print("HC-05 as Slave..");
  String remoteAddr;
  int rc = slaveConfig(remoteAddr);
  lcd.clear();
  if (rc < 0) {  // error
    lcd.print("ERROR: #"); lcd.print(rc);
    lcd.setCursor(0,1); lcd.print("Press BACK...");
    while (!menu.isBackButtonPressed());  // wait
    return NULL;
  }
  else {
    lcd.print("Success.");
    delay(2000);
  }

  //                             0123456789012345
  lcd.clear();         lcd.print("Insert Master");
  lcd.setCursor(0, 1); lcd.print("Press SELECT..");
  while (!menu.isSelectButtonPressed() && !menu.isBackButtonPressed());  // wait
  if (menu.isBackButtonPressed()) return NULL;
  
  //                               0123456789012345
  lcd.clear();          lcd.print("Programming");
  lcd.setCursor(0,1);   lcd.print("HC-05 as Master.");
  rc = masterConfig(remoteAddr);
  lcd.clear();
  if (rc < 0) {  // error
    lcd.print("ERROR: #"); lcd.print(rc);
    lcd.setCursor(0,1); lcd.print("Press BACK...");
    while (!menu.isBackButtonPressed());  // wait
    return NULL;
  }
  else {
    lcd.print("Success.");
  }
  
  lcd.setCursor(0,1); lcd.print("Press BACK...");
  while (!menu.isBackButtonPressed());  // wait
  return NULL;  
}

menuFuncPtr pair(const LCDMenuController *) {
  lcd.setCursor(0, 1);
  lcd.print(F("Pairing..."));

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
