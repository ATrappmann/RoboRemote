// NAME: RoboRemote_HW_Test.ino
//
// DESC: Test the wiring of Trappmann-Robotics RoboRemote bluetooth remote control.
//       see https://www.youtube.com/watch?v=xHsCnpfgwEI for Part 1
//       and https://www.youtube.com/watch?v=I3VOlUwofhE for Part 2
//
// SOURCE: Code is available at https://github.com/ATrappmann/RoboRemote
//
// USES LIBRARIES:
//  I2C Liquid Crystal from https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
// or
//  I2C LiquidCrystal from https://github.com/ATrappmann/LiquidCrystal_MCP23017_I2C
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

/*
 * Configuration
 */
// Check, which chip is used on the I2C expander module for the LCD.
// If the MCP23017 chip is used, uncomment the following line and use my library.
//#define USE_MCP23017_LCD

/*
 * Implementation
 */
#ifndef USE_MCP23017_LCD
// I2C LiquidCrystal library from https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
// Default is I2C-address 0x27.
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#else // use library for MCP23017 chip
// I2C LiquidCrystal libray from https://github.com/ATrappmann/LiquidCrystal_MCP23017_I2C
#include "LiquidCrystal_MCP23017_I2C.h"
#define LCD_RS_PIN  MCP23017_PA7
#define LCD_RW_PIN  MCP23017_PA6
#define LCD_EN_PIN  MCP23017_PA5
#define LCD_D0_PIN  MCP23017_PB0
#define LCD_D1_PIN  MCP23017_PB1
#define LCD_D2_PIN  MCP23017_PB2
#define LCD_D3_PIN  MCP23017_PB3
#define LCD_D4_PIN  MCP23017_PB4
#define LCD_D5_PIN  MCP23017_PB5
#define LCD_D6_PIN  MCP23017_PB6
#define LCD_D7_PIN  MCP23017_PB7
#define LCD_BACKLIGHT_PIN MCP23017_PA1

#define LCD_I2C_ADDR  0x20
LiquidCrystal_MCP23017_I2C lcd(LCD_I2C_ADDR, LCD_RS_PIN, LCD_RW_PIN, LCD_EN_PIN, LCD_BACKLIGHT_PIN,
                               LCD_D0_PIN, LCD_D1_PIN, LCD_D2_PIN, LCD_D3_PIN,
                               LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
#endif

void setup() {
  /*
   * Define I/O mode of digital pins D0-D13
   */
  // skip pins 0&1 for HardwareSerial
  
  // pins 2-9 are for digital inputs
  for (int pin=2; pin<=9; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }
  // pins 10-13 are for digital outputs
  for (int pin=10; pin<=13; pin++) {
    pinMode(pin, OUTPUT);
  }

  /*
   * Define I/O mode of analog pins A0-A7
   */
  // pins A6, A1-A2 are for analog inputs of joystick
  pinMode(A6, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  // pin A3 is for digital input of joystick
  pinMode(A3, INPUT_PULLUP);
  
  // skip pins A4&A5 for I2C communication
  
  // pin A0 is for HC-05 bluetooth pin 34
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  
  // pin A7 is for analog input of potentiometer
  pinMode(A7, INPUT);

  /* 
   * Start serial communication 
   */
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("RoboRemote HW-Test running..."));
  
  /*
   * Initialize LCD
   */
  lcd.begin(16, 2);
  lcd.print("Hello World!");
}

unsigned long lastTime = 0L;
void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastTime > 500) { // update every 500ms only
    lastTime = currentTime;

    // show digital inputs of pins 2-9
    for (int pin=2; pin<=9; pin++) {
      Serial.print(digitalRead(pin)); Serial.print(" ");
    }

    // toggle digital outputs of pins 10-13, all LEDs should be blinking
    for (int pin=10; pin<=13; pin++) {
      digitalWrite(pin, !digitalRead(pin));
    }
  
    // show Joystick potentiometer values and joystick button state
    Serial.print( analogRead(A6)); Serial.print(" ");
    Serial.print( analogRead(A1)); Serial.print(" ");
    Serial.print( analogRead(A2)); Serial.print(" ");
    Serial.print(digitalRead(A3)); Serial.print(" ");
  
    // show Bluetooth pin 34 state
    Serial.print(digitalRead(A0)); Serial.print(" ");
  
    // show additional potentiometer value
    Serial.print( analogRead(A7)); Serial.print(" ");
    Serial.println();

    // show seconds after startup on LCD
    lcd.setCursor(0, 1);
    lcd.print(millis() / 1000);  
  }
}
