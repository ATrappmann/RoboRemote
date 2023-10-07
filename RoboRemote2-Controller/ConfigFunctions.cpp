// NAME: ConfigFunctions.cpp
//

#include "ConfigFunctions.h"
#include "Bluetooth.h"

menuFuncPtr pairFunc(const LCDMenuController *) {
  response = sendCommand("AT+RESET");
  status = getStatus(response);
  if (!status.equals("OK")) return NULL;
  delay(1000);

  response = sendCommand("AT+CMODE=0");
  status = getStatus(response);
  if (!status.equals("OK")) return NULL;

  response = sendCommand("AT+INQM=0,5,9");
  status = getStatus(response);
  if (!status.equals("OK")) return NULL;

  response = sendCommand("AT+INIT");
  status = getStatus(response);
  if (!status.equals("OK")) return NULL;

  response = sendCommand("AT+INQ");
  status = getStatus(response);
  if (!status.equals("OK")) return NULL;

  // got list of bluetooth devices in format "+INQ:xxxx:yy:zzzzzz,aaaa,bbbb"
  String deviceList = getAnswer(response);
  do {
    int idx = deviceList.indexOf("+INQ:");
    if (-1 == idx) break;
    deviceList = deviceList.substring(idx+5);  // cut of prefix "+INQ:", len=5
    idx = deviceList.indexOf(',');  // find ',' behind BT address
    if (-1 == idx) return -7; // internal error
    String remoteAddr = deviceList.substring(0, idx);
    remoteAddr.replace(':', ',');

    String cmd = "AT+RNAME?" + remoteAddr;
    response = sendCommand(cmd.c_str());
    status = getStatus(response);
    if (!status.equals("OK")) return NULL;

    String answer = getAnswer(response);
    remoteName = answer.substring(7); // cut of prefix "+RNAME:", len=7

    if (remoteName.equals("RoboRemote")) {  // found RoboRemote
      cmd = "AT+PAIR=" + remoteAddr + ",9";
      response = sendCommand(cmd.c_str());
      status = getStatus(response);
      if (!status.equals("OK")) return NULL;

      cmd = "AT+BIND=" + remoteAddr;
      response = sendCommand(cmd.c_str());
      status = getStatus(response);
      if (!status.equals("OK")) return NULL;

      cmd = "AT+LINK=" + remoteAddr;
      response = sendCommand(cmd.c_str());
      status = getStatus(response);
      if (!status.equals("OK")) return NULL;

      break; // done
    }
  } while (-1 != deviceList.indexOf('\n'));
  return NULL;
}

menuFuncPtr progSlave(const LCDMenuController *){
  //                             0123456789012345
  lcd.clear();         lcd.print("HC-05 as Slave");
  lcd.setCursor(0, 1); lcd.print("Press SELECT..");
  while (!menu.isSelectButtonPressed() && !menu.isBackButtonPressed());  // wait
  if (menu.isBackButtonPressed()) return NULL;

  //                             0123456789012345
  lcd.clear();         lcd.print("Insert HC-05");
  lcd.setCursor(0, 1); lcd.print("Press SELECT..");
  while (!menu.isSelectButtonPressed() && !menu.isBackButtonPressed());  // wait
  if (menu.isBackButtonPressed()) return NULL;

  //                             0123456789012345
  lcd.clear();        lcd.print("Programming");
  lcd.setCursor(0,1); lcd.print("HC-05 as Slave..");

  delay(5000); // simulation

  //                             0123456789012345
  lcd.clear();        lcd.print("Done.");
  lcd.setCursor(0,1); lcd.print("Press BACK...");
  return NULL;
}

menuFuncPtr progMaster(const LCDMenuController *) {
  //                             0123456789012345
  lcd.clear();         lcd.print("HC-05 as Master");
  lcd.setCursor(0, 1); lcd.print("Press SELECT..");
  while (!menu.isSelectButtonPressed() && !menu.isBackButtonPressed());  // wait
  if (menu.isBackButtonPressed()) return NULL;

  //                             0123456789012345
  lcd.clear();         lcd.print("Insert HC-05");
  lcd.setCursor(0, 1); lcd.print("Press SELECT..");
  while (!menu.isSelectButtonPressed() && !menu.isBackButtonPressed());  // wait
  if (menu.isBackButtonPressed()) return NULL;

  //                             0123456789012345
  lcd.clear();        lcd.print("Programming");
  lcd.setCursor(0,1); lcd.print("HC-05 as Master");

  delay(5000); // simulation

  //                             0123456789012345
  lcd.clear();        lcd.print("Done.");
  lcd.setCursor(0,1); lcd.print("Press BACK...");
  return NULL;
}
