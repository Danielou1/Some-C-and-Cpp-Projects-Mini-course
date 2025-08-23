/*
 ***************************************************************************************************************************
 * @author: David Lotz
 * @file:   I2C_LCD.h
 * @date:   20.09.2022
 *
 * This module uses the I2C-Bus to control a HD44780 1602 LCD via the HW-061 I2C-Serial Interface with PCF8574 I/O Expander.
 *
 * *************************************************************************************************************************
 *
 * Lecturer:
 * Jakob Czekansky (jakob.czekansky@mni.thm.de)
 *
 * Mikroprozessortechnik
 * Technische Hochschule Mittelhessen
 *
 ***************************************************************************************************************************
 
 Connections:
 VCC - 5V
 GND - GND
 SDA - PA2
 SCL - PA3
 
 Call lcd_init() before using any other function.
 */


#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "../AVR128DB48_I2C/AVR128DB48_I2C.h"
#include <stdbool.h> // Keep for bool type if not using C++ <cstdbool>

i2c_status lcd_init(); // Removed void from parameter list for C++
i2c_status lcd_enable(bool enable);
i2c_status lcd_clear(); // Removed void from parameter list for C++
i2c_status lcd_moveCursor(uint8_t x, uint8_t y);
i2c_status lcd_backlight(bool enable);
i2c_status lcd_putChar(char character);
i2c_status lcd_putString(char* string);
i2c_status lcd_leftToRight(); // Removed void from parameter list for C++
i2c_status lcd_rightToLeft(); // Removed void from parameter list for C++

#endif /* I2C_LCD_H_ */