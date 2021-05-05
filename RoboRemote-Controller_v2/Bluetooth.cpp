// NAME: Bluetooth.cpp
//

#include "Bluetooth.h"

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

int slaveConfig(String& remoteAddr) {
  // check for command mode
  String response = sendCommand("AT");
  String status = getStatus(response);
  if (!status.equals("OK")) return -1;  // not in AT command mode

  // reset to factory defaults
  response = sendCommand("AT+ORGL");
  status = getStatus(response);
  if (!status.equals("OK")) return -2;  // error

  // set slave mode
  response = sendCommand("AT+ROLE=0");
  status = getStatus(response);
  if (!status.equals("OK")) return -3;  // error

  // reset after role change
  response = sendCommand("AT+RESET");
  status = getStatus(response);
  if (!status.equals("OK")) return -4;  // error

  // set name
  response = sendCommand("AT+NAME=RoboReceiver");
  status = getStatus(response);
  if (!status.equals("OK")) return -5;  // error

  // set password
  response = sendCommand("AT+PSWD=2206");
  status = getStatus(response);
  if (!status.equals("OK")) {
    response = sendCommand("AT+PSWD=\"2206\"");
    status = getStatus(response);
    if (!status.equals("OK")) return -6;  // error
  }

  // get address of module
  response = sendCommand("AT+ADDR?");
  status = getStatus(response);
  if (!status.equals("OK")) return -7;  // error

  remoteAddr = getAnswer(response);
  int idx = remoteAddr.indexOf("+ADDR:");
  if (-1 == idx) return -8; // error, unexpected answer
  remoteAddr = remoteAddr.substring(idx+6);  // cut of prefix "+ADDR:", len=6
  remoteAddr.replace(':', ',');

  return 0; // done
}

int masterConfig(const String& remoteAddr) {
  // check for command mode
  String response = sendCommand("AT");
  String status = getStatus(response);
  if (!status.equals("OK")) return -1;  // not in AT command mode

  // reset to factory defaults
  response = sendCommand("AT+ORGL");
  status = getStatus(response);
  if (!status.equals("OK")) return -2;  // error

  // set slave mode
  response = sendCommand("AT+ROLE=1");
  status = getStatus(response);
  if (!status.equals("OK")) return -3;  // error

  // reset after role change
  response = sendCommand("AT+RESET");
  status = getStatus(response);
  if (!status.equals("OK")) return -4;  // error

  // set name
  response = sendCommand("AT+NAME=RoboController");
  status = getStatus(response);
  if (!status.equals("OK")) return -5;  // error

  // set password
  response = sendCommand("AT+PSWD=2206");
  status = getStatus(response);
  if (!status.equals("OK")) {
    response = sendCommand("AT+PSWD=\"2206\"");
    status = getStatus(response);
    if (!status.equals("OK")) return -6;  // error
  }

  // connect only to known partners
  response = sendCommand("AT+CMODE=0");
  status = getStatus(response);
  if (!status.equals("OK")) return -7;  // error

  // set name
  String bind = "AT+BIND=" + remoteAddr;
  response = sendCommand(bind.c_str());
  status = getStatus(response);
  if (!status.equals("OK")) return -8;  // error

  return 0; // done
}
