/*
 * Uebung9.c
 *
 * Stable LCD display of voltage and percentage from the photoresistor at PF2.
 * Reads the voltage and displays it on the LCD.
 * Created: 05.12.2023
 * Author : splas
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "I2C_LCD.h"

// Buffers for strings
char buffer[16];  // Buffer for voltage
char buffer1[16]; // Buffer for percentage

// Previous values for stable display
float prev_voltage = -1;   // To track voltage changes
float prev_percent = -1;   // To track percentage changes

// Prototypes
void init_ADC(void);
uint16_t read_ADC(void);
void update_lcd_if_changed(float voltage, float percent);
void float_to_ascii(float value, char *buf, int decimal_places, char unit);

// Initialize ADC
void init_ADC() {
    PORTF.DIRCLR = PIN2_bm;                    // Set PF2 as input (photoresistor)
    VREF.ADC0REF = VREF_REFSEL_VDD_gc;         // Use VDD (3.3V) as reference voltage
    ADC0.MUXPOS = ADC_MUXPOS_AIN18_gc;         // Select PF2 (AIN18) as ADC input channel
    ADC0.CTRLB = ADC_RESSEL_12BIT_gc;          // Configure ADC for 12-bit resolution
    ADC0.CTRLA = ADC_ENABLE_bm;                // Enable ADC
    ADC0.CTRLC = ADC_PRESC_DIV4_gc;            // Set prescaler (adjust ADC clock to be within spec)
}

// Read ADC value
uint16_t read_ADC() {
    ADC0.COMMAND = ADC_STCONV_bm;              // Start ADC conversion
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));  // Wait for conversion to complete
    ADC0.INTFLAGS = ADC_RESRDY_bm;             // Clear interrupt flag
    return ADC0.RES;                           // Return result
}

// Convert floating-point value to ASCII string
void float_to_ascii(float value, char *buf, int decimal_places, char unit) {
    int whole_part = (int)value;                        // Extract whole part
    int fractional_part = (int)((value - whole_part) * 10 * decimal_places); // Extract fractional part

    // Convert whole part
    int i = 0;
    if (whole_part == 0) {
        buf[i++] = '0';
    } else {
        char temp[8];
        int j = 0;
        while (whole_part > 0) {
            temp[j++] = (whole_part % 10) + '0';
            whole_part /= 10;
        }
        while (j > 0) {
            buf[i++] = temp[--j];
        }
    }

    // Add decimal point and fractional part
    buf[i++] = '.';
    for (int d = 0; d < decimal_places; d++) {
        buf[i++] = (fractional_part % 10) + '0';
        fractional_part /= 10;
    }

    // Add unit and null-terminate
    buf[i++] = unit;
    buf[i] = '\0';
}

// Update LCD only if values have changed
void update_lcd_if_changed(float voltage, float percent) {
    // Update voltage if it changed
    if (voltage != prev_voltage) {
        lcd_moveCursor(0, 0); // Move to the first line
        float_to_ascii(voltage, buffer, 2, 'V'); // Convert voltage to ASCII
        lcd_putString(buffer); // Display voltage
        prev_voltage = voltage; // Update previous value
    }

    // Update percentage if it changed
    if (percent != prev_percent) {
        lcd_moveCursor(0, 1); // Move to the second line
        float_to_ascii(percent, buffer1, 1, '%'); // Convert percentage to ASCII
        lcd_putString(buffer1); // Display percentage
        prev_percent = percent; // Update previous value
    }
}

int main(void) {
    // Initialize LCD and ADC
    lcd_init();
    lcd_enable(true);
    init_ADC();

    while (1) {
        uint16_t adcValue = read_ADC(); // Read the ADC value

        // Calculate percentage and voltage
        float percent = (adcValue * 100.0) / 4095;
        float voltage = (adcValue * 3.3) / 4095;

        // Update LCD only if necessary
        update_lcd_if_changed(voltage, percent);
    }
}
