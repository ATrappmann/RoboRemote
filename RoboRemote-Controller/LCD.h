// NAME: LCD.h
//
#ifndef LCD_H
#define LCD_H

// Check, which chip is used on the LCD I2C module.
// If the MCP23017 chip is used, uncomment the following line.
#define USE_MCP23017_LCD

#ifndef USE_MCP23017_LCD
// I2C LiquidCrystal library from https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
// Default is I2C-address 0x27.
#include <LiquidCrystal_I2C.h>
extern LiquidCrystal_I2C lcd;
#else
// I2C LiquidCrystal libray from https://github.com/ATrappmann/LiquidCrystal_MCP23017_I2C
#include "LiquidCrystal_MCP23017_I2C.h"
extern LiquidCrystal_MCP23017_I2C lcd;

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

#endif

#endif /* LCD_H */
