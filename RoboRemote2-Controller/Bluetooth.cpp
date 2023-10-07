// NAME: Bluetooth.cpp
//

#include "Bluetooth.h"
#include "LCD.h"
#include "RoboRemote_Pins.h"

String sendCommand(const char *cmd) {
  BTserial.write(cmd); BTserial.write("\r\n");

  String response = "";
  int len;
  char c;
  int loops = 0;
  do {
    len = BTserial.available();
    if (len > 0) {
      for (int i=0; i<len; i++) {
        c = BTserial.read();
        if ('\r' != c) {
          response += c; // skip CRs
        }
      }
    }
    else delay(100);
    len = response.length();
    if ((0 == len) && (++loops > 100)) return "";  // not in AT command mode?
  } while (BTserial.available() || (len < 3) || (c != '\n') ||
           (('K' != response[len-2]) && ('L' != response[len-2]) && (')' != response[len-2])));

  response = response.substring(0, response.length()-1);  // kill last LF
  return response;
}

String getStatus(const String& response) {
  int idx = response.lastIndexOf('\n');
  if (-1 == idx) {  // only status in response
    return response;
  }
  else return response.substring(idx+1);
}

String getAnswer(const String& response) {
  int idx = response.lastIndexOf('\n');
  if (-1 == idx) {  // only status in response
    return "";
  }
  else return response.substring(0, idx);
}

int BTconnect() {
  // check for command mode
  String response = sendCommand("AT");
  String status = getStatus(response);
  if (!status.equals("OK")) return -1;  // not in AT command mode

  String remoteName;
  response = sendCommand("AT+ADCN?");
  String answer = getAnswer(response);
  if (answer.equals("+ADCN:0")) { // no authenticated devices
    lcd.setCursor(12, 1);
    lcd.print("Init");

    response = sendCommand("AT+RESET");
    status = getStatus(response);
    if (!status.equals("OK")) return -2;
    delay(1000);

    response = sendCommand("AT+CMODE=0");
    status = getStatus(response);
    if (!status.equals("OK")) return -3;

    response = sendCommand("AT+INQM=0,5,9");
    status = getStatus(response);
    if (!status.equals("OK")) return -4;

    response = sendCommand("AT+INIT");
    status = getStatus(response);
    if (!status.equals("OK")) return -5;

    response = sendCommand("AT+INQ");
    status = getStatus(response);
    if (!status.equals("OK")) return -6;

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
      if (!status.equals("OK")) return -8;

      String answer = getAnswer(response);
      remoteName = answer.substring(7); // cut of prefix "+RNAME:", len=7

      if (remoteName.equals("RoboRemote")) {  // found RoboRemote
        cmd = "AT+PAIR=" + remoteAddr + ",9";
        response = sendCommand(cmd.c_str());
        status = getStatus(response);
        if (!status.equals("OK")) return -9;

        cmd = "AT+BIND=" + remoteAddr;
        response = sendCommand(cmd.c_str());
        status = getStatus(response);
        if (!status.equals("OK")) return -10;

        cmd = "AT+LINK=" + remoteAddr;
        response = sendCommand(cmd.c_str());
        status = getStatus(response);
        if (!status.equals("OK")) return -11;

        break; // done
      }
    } while (-1 != deviceList.indexOf('\n'));
  }
  else { // authenticated device found
    lcd.setCursor(11, 1);
    lcd.print("Bound");

    response = sendCommand("AT+RESET");
    status = getStatus(response);
    if (!status.equals("OK")) return -12;
    delay(1000);

    response = sendCommand("AT+INIT");
    status = getStatus(response);
    if (!status.equals("OK")) return -13;
/*
    response = sendCommand("AT+MRAD?");
    status = getStatus(response);
    if (!status.equals("OK")) return -14;

    String lastDevice = getAnswer(response);
    lastDevice = lastDevice.substring(6); // cut of prefix "+MRAD:", len=6
    lastDevice.replace(':', ',');

    String cmd = "AT+RNAME?" + lastDevice;
    response = sendCommand(cmd.c_str());
    status = getStatus(response);
    if (!status.equals("OK")) return -15;

    String answer = getAnswer(response);
    remoteName = answer.substring(7); // cut of prefix "+RNAME:", len=7
*/
  }

  digitalWrite(BT_CMD_PIN, LOW);  // switch to communication mode
  while (!digitalRead(BT_STATE_PIN)); // wait for connection
  return 0;
}
