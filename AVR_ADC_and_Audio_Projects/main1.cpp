#include <avr/io.h>
#include <stdbool.h> // Keep for bool type if not using C++ <cstdbool>
#include "I2C_LCD.h"

// Buffer fr Strings
char buffer[16];  
char buffer1[16]; 

// Vorherige Werte zur stabilen Anzeige
float prev_voltage = -1.0f; // Use .0f for float literals
float prev_percent = -1.0f;

/**
 * @brief Initialisiert den ADC fr PF3.
 * 
 * Konfiguriert den ADC mit VDD als Referenzspannung, 12-Bit-Auflsung und dem Eingangskanal PF3 (AIN19).
 */
void init_ADC(); // Removed void from parameter list for C++

/**
 * @brief Liest einen Wert vom ADC.
 * 
 * Startet eine ADC-Konvertierung und wartet, bis das Ergebnis verfgbar ist.
 * 
 * @return 12-Bit-Ergebnis der ADC-Konvertierung.
 */
uint16_t read_ADC(); // Removed void from parameter list for C++

/**
 * @brief Aktualisiert das LCD-Display nur, wenn sich Werte gendert haben.
 * 
 * Vergleicht die bergebenen Werte mit den vorherigen und aktualisiert das LCD
 * nur, wenn Unterschiede festgestellt werden.
 * 
 * @param voltage Aktuelle Spannung in Volt.
 * @param percent Aktueller Prozentsatz.
 */
void update_lcd_if_changed(float voltage, float percent);

/**
 * @brief Konvertiert einen Float-Wert in eine ASCII-Zeichenkette.
 * 
 * Wandelt einen Fliekommawert in eine Zeichenkette mit der angegebenen Anzahl an Dezimalstellen um
 * und fgt eine Einheit am Ende hinzu.
 * 
 * @param value Der zu konvertierende Wert.
 * @param buf Der Zielpuffer fr die Zeichenkette.
 * @param decimal_places Anzahl der Nachkommastellen.
 * @param unit Einheit, die an den Wert angehngt wird.
 */
void float_to_ascii(float value, char *buf, int decimal_places, char unit);

void init_ADC() {
    PORTF.DIRCLR = PIN3_bm;                    
    VREF.ADC0REF = VREF_REFSEL_VDD_gc;         
    ADC0.MUXPOS = ADC_MUXPOS_AIN19_gc;         
    ADC0.CTRLB = ADC_RESSEL_12BIT_gc;          
    ADC0.CTRLA = ADC_ENABLE_bm;                
    ADC0.CTRLC = ADC_PRESC_DIV4_gc;            
}

uint16_t read_ADC() {
    ADC0.COMMAND = ADC_STCONV_bm;              
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));  
    ADC0.INTFLAGS = ADC_RESRDY_bm;             
    return ADC0.RES;                           
}

void float_to_ascii(float value, char *buf, int decimal_places, char unit) {
    int whole_part = static_cast<int>(value); // C++ style cast                       
    int fractional_part = static_cast<int>((value - whole_part) * 10 * decimal_places); // C++ style cast

    int i = 0;
    if (whole_part == 0) {
        buf[i++] = '0';
    } else {
        char temp[8];
        int j = 0;
        while (whole_part > 0) {
            temp[j++] = static_cast<char>((whole_part % 10) + '0'); // C++ style cast
            whole_part /= 10;
        }
        while (j > 0) {
            buf[i++] = temp[--j];
        }
    }

    buf[i++] = '.';
    for (int d = 0; d < decimal_places; d++) {
        buf[i++] = static_cast<char>((fractional_part % 10) + '0'); // C++ style cast
        fractional_part /= 10;
    }

    buf[i++] = unit;
    buf[i] = '\0';
}

void update_lcd_if_changed(float voltage, float percent) {
    if (voltage != prev_voltage) {
        lcd_moveCursor(0, 0); 
        float_to_ascii(voltage, buffer, 2, 'V'); 
        lcd_putString(buffer); 
        prev_voltage = voltage; 
    }

    if (percent != prev_percent) {
        lcd_moveCursor(0, 1); 
        float_to_ascii(percent, buffer1, 2, '%'); 
        lcd_putString(buffer1); 
        prev_percent = percent; 
    }
}

int main() { // Changed from main(void) to int main()
    lcd_init();
    lcd_enable(true);
    init_ADC();

    while (true) { // Use true instead of 1 for C++
        uint16_t adcValue = read_ADC(); 

        float percent = (adcValue * 100.0f) / 4095.0f; // Use .0f for float literals
        float voltage = (adcValue * 3.3f) / 4095.0f; // Use .0f for float literals

        update_lcd_if_changed(voltage, percent);
    }
    return 0; // Added return 0 for int main()
}