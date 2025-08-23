#include <avr/io.h>
#include <stdio.h> // Keep for sprintf
#include "I2C_LCD.h"
#include "AVR128DB48_I2C.h"

#define F_CPU 4000000UL ///< CPU frequency
#include <util/delay.h>
#define TCS34725_ADDRESS 0x29 ///< TCS34725 I2C address
#define SIZE 10 ///< Buffer size for displaying values

/**
 * @brief Global variables to store sensor data.
 */
uint8_t read_bits[8]; ///< Buffer to store raw sensor data
uint16_t clear_val, red_val, green_val, blue_val = 0; // Renamed to avoid conflict with color names
char color_buf[SIZE] = ""; ///< Buffer for formatted display strings

/**
 * @brief Initialize the TCS34725 sensor.
 *
 * This function configures the TCS34725 sensor by enabling it, setting the 
 * integration time, and configuring the gain. It sends the appropriate I2C commands
 * to the sensor to set these parameters.
 */
void i2c_initialize(); // Removed void from parameter list for C++

/**
 * @brief Main program loop.
 *
 * The program initializes the LCD and the TCS34725 sensor, then continuously 
 * reads color data from the sensor and displays the values on the LCD.
 *
 * @return int Returns 0 on successful execution (not used in embedded systems).
 */
int main() { // Changed from main(void) to int main()
    lcd_init();          ///< Initialize LCD
    lcd_enable(true);    ///< Enable LCD
    i2c_initialize();    ///< Initialize the TCS34725 sensor

    while (true) { // Use true instead of 1 for C++
        // Request data from the sensor
        uint8_t reg = static_cast<uint8_t>(0x80 | 0x14); ///< COMMAND_BIT + STARTING REGISTER
        i2c_write(TCS34725_ADDRESS, &reg, 1);
        _delay_ms(30);
        i2c_read(TCS34725_ADDRESS, read_bits, 8);

        // Combine high and low bytes to form color values
        clear_val = static_cast<uint16_t>((read_bits[1] << 8) | read_bits[0]);
        red_val   = static_cast<uint16_t>((read_bits[3] << 8) | read_bits[2]);
        green_val = static_cast<uint16_t>((read_bits[5] << 8) | read_bits[4]);
        blue_val  = static_cast<uint16_t>((read_bits[7] << 8) | read_bits[6]);

        // Display values on LCD
        lcd_clear();

        lcd_moveCursor(0, 0);
        sprintf(color_buf, "C:%d", clear_val);
        lcd_putString(color_buf);

        lcd_moveCursor(9, 0);
        sprintf(color_buf, "R:%d", red_val);
        lcd_putString(color_buf);

        lcd_moveCursor(0, 1);
        sprintf(color_buf, "G:%d", green_val);
        lcd_putString(color_buf);

        lcd_moveCursor(9, 1);
        sprintf(color_buf, "B:%d", blue_val);
        lcd_putString(color_buf);

        _delay_ms(500); ///< Delay to avoid excessive updates
    }
    return 0; // Added return 0 for int main()
}

void i2c_initialize() { // Removed void from parameter list for C++
    uint8_t enable_bits[] = {static_cast<uint8_t>(0x80 | 0x00), 0x03}; ///< Enable register: PON | AEN
    i2c_write(TCS34725_ADDRESS, enable_bits, 2);
    _delay_ms(30); // Allow time for enabling

    uint8_t atime_bits[] = {static_cast<uint8_t>(0x80 | 0x01), 0xD5}; ///< ATIME register: 101 ms
    i2c_write(TCS34725_ADDRESS, atime_bits, 2);
    _delay_ms(30);

    uint8_t gain_bits[] = {static_cast<uint8_t>(0x80 | 0x0F), 0x01}; ///< CONTROL register: Gain = 4x
    i2c_write(TCS34725_ADDRESS, gain_bits, 2);
    _delay_ms(30);
}
