// NAME: RoboRemote2-Controller.ino
//
// DESC: Sketch for RoboRemote bluetooth remote controller.
//       see https://www.youtube.com/watch?v=xHsCnpfgwEI for Part 1
//       and https://www.youtube.com/watch?v=I3VOlUwofhE for Part 2
//       and https://www.youtube.com/watch?v=15aVeGGLiDI for Part 3
//
// SOURCE: Code is available at https://github.com/ATrappmann/RoboRemote/Controller
//
// USES LIBRARIES:
//  I2C Liquid Crystal from https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
// or
//  I2C LiquidCrystal from https://github.com/ATrappmann/LiquidCrystal_MCP23017_I2C
// and
//  EasyTransfer from https://github.com/madsci1016/Arduino-EasyTransfer
//
// MIT License
//
// Copyright (c) 2020 Andreas Trappmann
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#define MAJOR 2
#define MINOR 0

#include <EasyTransfer.h>

#include "RoboRemote_Pins.h"
#include "Bluetooth.h"
#include "LCDMenuController.h"

#define LCD_I2C_ADDR  0x20
LiquidCrystal_MCP23017_I2C lcd = LiquidCrystal_MCP23017_I2C(LCD_I2C_ADDR);
LCDMenuController menu = LCDMenuController(&lcd, 16, 2, DOWN_BTN_PIN, UP_BTN_PIN, ENTER_BTN_PIN, GREEN_BTN_PIN);

EasyTransfer ET;

#define RIGHT_BTN_MASK  (1<<0)
#define UP_BTN_MASK     (1<<1)
#define ENTER_BTN_MASK  (1<<2)
#define DOWN_BTN_MASK   (1<<3)
#define LEFT_BTN_MASK   (1<<4)
#define RED_BTN_MASK    (1<<5)
#define GREEN_BTN_MASK  (1<<6)
#define JSK_BTN_MASK    (1<<7)

struct SEND_DATA {
  uint8_t   major;
  uint8_t   minor;
  uint32_t  packetNo;
  uint8_t   buttons;
  uint16_t  jsk_roll;
  uint16_t  jsk_pitch;
  uint16_t  jsk_yaw;
  uint16_t  pot;
} txData;

Menu configMenu[] = {
  //        0123456789012345
  HEADLINE("Config Menu"),
  FUNCCALL("Pair", pairFunc),
  FUNCCALL("Prg. HC05 Slave", progSlave),
  FUNCCALL("Prg. HC05 Master", progMaster),
  ENDOFMENU()
};

Menu mainMenu[] = {
  //        0123456789012345
  HEADLINE("RoboRemote v2"),
  FUNCCALL("Start", startRemote),
   SUBMENU("Config", configMenu),
  ENDOFMENU()
};

menuFuncPtr startRemote(const LCDMenuController *controller) {
  if (menu.isSelectButtonPressed()) return NULL;  // done

  if (!digitalRead(BT_STATE_PIN)) { // not paired
    digitalWrite(BLUE_LED_PIN, LOW);  // switch of blue LED
    while (!digitalRead(BT_STATE_PIN)); // wait for pairing
  }
  else digitalWrite(BLUE_LED_PIN, HIGH);  // paired

  txData.packetNo++;
  txData.buttons = 0;
  if (digitalRead(RIGHT_BTN_PIN)) txData.buttons |= RIGHT_BTN_MASK;
  if (digitalRead(UP_BTN_PIN))    txData.buttons |= UP_BTN_MASK;
  if (digitalRead(ENTER_BTN_PIN)) txData.buttons |= ENTER_BTN_MASK;
  if (digitalRead(DOWN_BTN_PIN))  txData.buttons |= DOWN_BTN_MASK;
  if (digitalRead(LEFT_BTN_PIN))  txData.buttons |= LEFT_BTN_MASK;
  if (digitalRead(RED_BTN_PIN))   txData.buttons |= RED_BTN_MASK;
  if (digitalRead(GREEN_BTN_PIN)) txData.buttons |= GREEN_BTN_MASK;
  if (digitalRead(JSK_BTN_PIN))   txData.buttons |= JSK_BTN_MASK;
  txData.jsk_roll = analogRead(JSK_ROLL_PIN);
  txData.jsk_pitch= analogRead(JSK_PITCH_PIN);
  txData.jsk_yaw  = analogRead(JSK_YAW_PIN);
  txData.pot = analogRead(POT_PIN);
  ET.sendData();

  controller->getDisplay()->setCursor(0, 1);
  controller->getDisplay()->print(txData.packetNo);

  return startRemote;
}

void setup() {
  pinMode(RIGHT_BTN_PIN, INPUT_PULLUP);
  pinMode(UP_BTN_PIN, INPUT_PULLUP);
  pinMode(ENTER_BTN_PIN, INPUT_PULLUP);
  pinMode(DOWN_BTN_PIN, INPUT_PULLUP);
  pinMode(LEFT_BTN_PIN, INPUT_PULLUP);
  pinMode(RED_BTN_PIN, INPUT_PULLUP);
  pinMode(GREEN_BTN_PIN, INPUT_PULLUP);

  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  pinMode(BT_CMD_PIN, OUTPUT);
  pinMode(BT_STATE_PIN, INPUT);

  pinMode(JSK_ROLL_PIN, INPUT);
  pinMode(JSK_PITCH_PIN, INPUT);
  pinMode(JSK_YAW_PIN, INPUT);
  pinMode(JSK_BTN_PIN, INPUT_PULLUP);

  pinMode(POT_PIN, INPUT);

  digitalWrite(BT_CMD_PIN, HIGH); // was HIGH during reset anyway
  digitalWrite(BLUE_LED_PIN, LOW);

  // init LCD
  lcd.begin(16, 2);
  lcd.print(F("RoboRemote ")); lcd.print(MAJOR); lcd.print('.'); lcd.print(MINOR);
  lcd.setCursor(0, 1);
  lcd.print(F("Pairing..."));

  // start communication with the HC-05
  BTserial.begin(BTspeed);
  int status = BTconnect();
  if (status < 0) {
    digitalWrite(RED_LED_PIN, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(F("ERROR: #")); lcd.print(status);
    exit(-1);
  }

  digitalWrite(BLUE_LED_PIN, HIGH);
  lcd.setCursor(0, 1);
  lcd.print(F("Paired!         "));
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print(F("       "));

  ET.begin(details(txData), &BTserial);
  txData.major = MAJOR;
  txData.minor = MINOR;
  txData.packetNo = 0L;

  menu.init();
  menu.begin(mainMenu);
}

void loop() {
  menu.navigate();
}
