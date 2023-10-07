// NAME: LCD.cpp
//

#include "LCD.h"

#ifdef USE_MCP23017_LCD
LiquidCrystal_MCP23017_I2C lcd(LCD_I2C_ADDR, LCD_RS_PIN, LCD_RW_PIN, LCD_EN_PIN, LCD_BACKLIGHT_PIN,
                               LCD_D0_PIN, LCD_D1_PIN, LCD_D2_PIN, LCD_D3_PIN,
                               LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
#else
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);
#endif
