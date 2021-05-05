// Basic Bluetooth sketch HC-05
// Connect the HC-05 module and communicate using the serial monitor
//
// The HC-05 defaults to communication mode when powered on. Needs to be placed into
// AT mode by either pressing the micro switch or setting pin34 to 3,3V while powering
// on the device.
// In AT mode, all commands must be transmitted with CR and LF.
// After a factory reset the default baud rate for communication mode is 38400.
//
// Pins:
//  BT VCC to Arduino 5V out.
//  BT GND to GND
//  BT STATE to Arduino pin 4 (10K pull-down resistor to GND)
//  BT RX to Arduino pin 3 (through a voltage divider)
//  BT TX to Arduino pin 2 (no need for voltage divider)
//  BT EN/KEY or pin34 to Arduino pin 5 (through a voltage divider)
//
// Check accessibility and configuration:
//  AT          ->  OK
//  AT+VERSION? ->  +VERSION:2.0-<date> or +VERSION:3.0-<date>
//  AT+STATE?   ->  +STATE:INITIALIZED
//  AT+UART?    ->  +UART:9600,0,0
//  AT+ROLE?    ->  +ROLE:0 (slave) of +ROLE:1 (master)
//  AT+CMODE?   ->  +CMOD:0 (connect only to known devices) of +CMOD:1 (connect to any device)
//  AT+NAME?    ->  +NAME:HC-05 (or some custom name)
//  AT+PSWD?    ->  +PSWD:xxxx (version 2) or +PSWD:"xxxx" (version 3)
//  AT+ADDR?    ->  +ADDR:xxxx:yy:zzzzzz (our BT address)
//  AT+ADCN?    ->  +ADCN:x (get authenticated device count)
//
// Reset to factory defaults:
//  AT+ORGL     ->  OK
//
// Configuration as Slave (RoboRemote Receiver)
//  AT+ORGL             ->  OK (restore factory settings, HC05: slave mode, 38400)
//  AT+ROLE=0           ->  OK (slave)
//  AT+RESET            ->  OK (only after ROLE change)
//  AT+UART=38400,0,0   ->  OK (set communication speed)
//  AT+NAME=RoboRemote  ->  OK (set our name)
//  AT+PSWD=xxxx        ->  OK (define the PIN, if ERROR:(1D), try AT+PSWD="xxxx")
//
// Configuration as Master (RoboRemote Transmitter)
//  AT+ORGL                   ->  OK (restore factory settings, HC05: slave mode, 38400)
//  AT+ROLE=1                 ->  OK (master)
//  AT+RESET                  ->  OK (only after ROLE change)
//  AT+UART=38400,0,0         ->  OK (set communication speed)
//  AT+NAME=RoboController    ->  OK (set our name)
//  AT+PSWD=xxxx              ->  OK (set PIN of slave, if ERROR:(1D), try AT+PSWD="xxxx" (version 3))
//  --AT+RMAAD                  ->  OK (delete all authenticated devices)
//  AT+CMODE=0                ->  OK (connect only to specified devices)
//  AT+INQM=0,5,9             ->  OK (set inquire access mode, max 5 devices in 9s)
//  --AT+INIT                   ->  OK (initialize SPP)
//  AT+INQ                    ->  +INQ:xxxx:yy:zzzzzz,aaaa,bbbb (get list of devices in range)
//  AT+RNAME?xxxx,yy,zzzzzz   ->  +RNAME:RoboRemote (ask for name of specified device)
//  AT+PAIR=xxxx,yy,zzzzzz,9  ->  OK (set pair with specified device, timeout in 9s)
//  AT+BIND=xxxx,yy,zzzzzz    ->  OK (set BT address)
//  AT+LINK=xxxx,yy,zzzzzz    ->  OK (connect device, changes to communication mode)
//

//#define DEBUG 1
#include <TrappmannRobotics.h>

#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
#define STATE_PIN 4
#define CMD_PIN   5

#define LED_PIN LED_BUILTIN

String sendCommand(const char *cmd) {
  Serial.print(">"); Serial.println(cmd);
  BTserial.write(cmd); BTserial.write("\r\n");

  String response = "";
  int len;
  char c;
  int loops = 0;
  do {
    len = BTserial.available();
    if (len > 0) {
      SEROUT(F("available=") << len << LF);
      for (int i=0; i<len; i++) {
        c = BTserial.read();
        if ('\r' != c) {
          response += c; // skip CRs
          Serial.write(c);
        }
      }
    }
    else delay(20);
    len = response.length();
    if ((0 == len) && (++loops > 100)) return "";  // not in AT command mode?
  } while (BTserial.available() || (len < 3) || (c != '\n') ||
           (('K' != response[len-2]) && ('L' != response[len-2]) && (')' != response[len-2])));

  response = response.substring(0, response.length()-1);  // kill last LF
  SEROUT(F("loops=") << loops << ", response='" << response << "'\n");
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

void setup() {
  pinMode(STATE_PIN, INPUT);
  pinMode(CMD_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(CMD_PIN, HIGH); // was HIGH during reset anyway
  digitalWrite(LED_PIN, LOW);

  // start the serial communication with the host computer
  Serial.begin(38400);
  while (!Serial);
  Serial.println(F("Arduino with HC-05 terminal is ready"));

  // start communication with the HC-05 using 38400
  BTserial.begin(38400);
  Serial.println(F("BTserial started at 38400"));
  Serial.println(F("HC-05 LED blinking modes:"));
  Serial.println(F("blink with 0,5Hz: full AT command mode (EN or pin34 is HIGH)"));
  Serial.println(F("blink with 5Hz  : communication mode, not paired"));
  Serial.println(F("blink with 1Hz  : communication mode, connecting"));
  Serial.println(F("twice every 2-3s: communication mode, paired"));

/*
  // check for command mode
  String response = sendCommand("AT");
  String status = getStatus(response);
  if (!status.equals("OK")) {
    Serial.println(F("ERROR: Bluetooth module HC-05 is not in AT command mode!"));
    Serial.println(F("Exit. Press RESET to continue..."));
    Serial.flush();
    exit(-1);
  }
*/

  Serial.println("Ready.");
}

bool NL = true;
void loop() {
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  while (BTserial.available()) {
    char c = BTserial.read();
    Serial.write(c);
  }

  // Keep reading from Arduino Serial Monitor and send to HC-05
  while (Serial.available()) {
    char c =  Serial.read();
    BTserial.write(c);

    // Echo the user input to the main window. The ">" character indicates the user entered text.
    if (NL) {
      Serial.print(">");
      NL = false;
    }
    Serial.write(c);
    if (c==10) NL = true;
  }

  if (digitalRead(STATE_PIN)) {
    digitalWrite(LED_PIN, HIGH);  // paired
  }
  else digitalWrite(LED_PIN, LOW);  // not paired
}
