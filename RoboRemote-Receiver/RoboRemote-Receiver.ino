// NAME: RoboRemote-Receiver.ino
//
// DESC: Demo receiver for RoboRemote bluetooth controller.
//       Prints out all packets received to Serial with 115200 baud.
//
// PINS:
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  BT STATE to Arduino pin 4 (10K pull-down resistor to GND)
//  BT RX to Arduino pin 3 (through a voltage divider)
//  BT TX to Arduino pin 2 (no need for voltage divider)
//
// USES LIBRARIES:
//  EasyTransfer from https://github.com/madsci1016/Arduino-EasyTransfer
//  TrappmannRobotics-Library from https://github.com/ATrappmann/TrappmannRobotics-Library
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

#include <TrappmannRobotics.h>
#include <EasyTransfer.h>

EasyTransfer ET;

#define RIGHT_BTN_MASK  (1<<0)
#define UP_BTN_MASK     (1<<1)
#define ENTER_BTN_MASK  (1<<2)
#define DOWN_BTN_MASK   (1<<3)
#define LEFT_BTN_MASK   (1<<4)
#define RED_BTN_MASK    (1<<5)
#define GREEN_BTN_MASK  (1<<6)
#define JSK_BTN_MASK    (1<<7)

struct RECEIVE_DATA {
  uint8_t   major;
  uint8_t   minor;
  uint32_t  packetNo;
  uint8_t   buttons;
  uint16_t  jsk_roll;
  uint16_t  jsk_pitch;
  uint16_t  jsk_yaw;
  uint16_t  pot;
} rxData;

#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
#define STATE_PIN 4

void setup() {
  pinMode(STATE_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  
  // start the serial communication with the host computer
  Serial.begin(115200);
  while (!Serial);
  Serial << F("-------------------------------\n");
  Serial << F("Sketch: ") << getBaseName(__FILE__) << LF;
  Serial << F("Uploaded: ") << TrappmannRobotics::getUploadTimestamp() << LF;
  Serial << F("Free Memory: ") << TrappmannRobotics::getFreeMemory() << F(" bytes\n");
  Serial << F("Running RoboRemote-Receiver...\n");
  Serial << F("Wait for pairing...\n");

  // start communication with the HC-05 using 38400
  BTserial.begin(38400);  
  ET.begin(details(rxData), &BTserial);

  while(!digitalRead(STATE_PIN)); // wait for pairing
  Serial << F("Paired!\n");
  
  while (!ET.receiveData());  // wait for data
  Serial << F("RoboRemote: version=") << rxData.major << '.' << rxData.minor << LF;
  delay(1000);
    
  Serial << F("Ready for remote control...\n");
}

uint32_t lastTime = 0L;
void loop() {
  if (digitalRead(STATE_PIN)) {
    digitalWrite(LED_PIN, HIGH);  // paired
  }
  else digitalWrite(LED_PIN, LOW);  // not paired

  if (ET.receiveData()) {
    uint32_t currentTime = millis();
    Serial << F("cnt=") << rxData.packetNo
           << F(", btns=") << toBinaryString(rxData.buttons)
           << F(", roll=") << rxData.jsk_roll
           << F(", pitch=") << rxData.jsk_pitch
           << F(", yaw=") << rxData.jsk_yaw
           << F(", pot=") << rxData.pot
           << F(", dt=") << (currentTime - lastTime) << LF;
    lastTime = currentTime;
  }
}
