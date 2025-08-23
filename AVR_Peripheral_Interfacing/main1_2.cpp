/*************************************************************************************************
 * @file main1_2.c
 * @brief Steuerung einer LED mit zuflliger Farbe mittels Interrupts und Pseudozufallszahlen.
 * 
 * Dieses Programm schaltet eine LED ein oder aus, wenn ein Taster bettigt wird. 
 * Wenn die LED eingeschaltet wird, erhlt sie eine zufllige Farbe basierend 
 * auf einem Xorshift32-Pseudozufallszahlengenerator.
 * 
 * @author Danielou Mouns
 * @date 02.12.2024
 ********************************************************************************************/

#include "main.h"

// Globale Variablen
/** 
 * @brief Zustand der LED: 0 = aus, 1 = ein.
 */
volatile uint8_t state = 0; 

/**
 * @brief Zustand fr den Xorshift32-Pseudozufallszahlengenerator.
 */
uint32_t xorshift_state = 0x016;

/**
 * @brief Generiert eine Pseudozufallszahl mit Xorshift32.
 * 
 * Ein schneller Algorithmus zur Erzeugung von 32-Bit-Zufallszahlen. 
 * Der Ausgangswert wird verwendet, um die Farbe der LED zu bestimmen.
 * 
 * @return 32-Bit-Pseudozufallszahl.
 */
uint32_t xorshift32() {
    xorshift_state ^= (xorshift_state << 13);
    xorshift_state ^= (xorshift_state >> 17);
    xorshift_state ^= (xorshift_state << 5);
    return xorshift_state;
}

/**
 * @brief Interrupt-Service-Routine fr den Button an Pin C4.
 * 
 * Diese Routine schaltet die LED ein oder aus. Wenn die LED eingeschaltet wird, 
 * erhlt sie eine zufllige Farbe basierend auf der Ausgabe des `xorshift32`-Generators.
 * Nach der Verarbeitung wird das Interrupt-Flag zurckgesetzt.
 */

ISR(PORTC_PORT_vect) {
    _delay_ms(WAITING_FOR_DEBOUNCING);
    if (state == 0) {
        PORTE.OUTCLR = LED_PINS; 
    } else {
        PORTE.OUTSET = static_cast<uint8_t>(xorshift32() & LED_PINS); // Cast to uint8_t
    }
    state ^= 1; // Zustand umschalten
    PORTC.INTFLAGS = PIN4_bm; // Interrupt-Flag lschen
}

/**
 * @brief Hauptprogramm zur Initialisierung und Steuerung der LED.
 * 
 * Initialisiert die LED-Pins, den Taster und die Interrupts. Das Programm luft 
 * in einer Endlosschleife und reagiert ausschlielich auf die Interrupts.
 * 
 * @return Kehrt nicht zurck.
 */

int main() { // Changed from main(void) to int main()
    PORTE.DIRSET = LED_PINS;
    PORTC.PIN4CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN4CTRL = PORT_ISC_RISING_gc;
    sei();

    while (true) { // Use true instead of 1 for C++
        // Wartet passiv auf Interrupts
    }
    return 0; // Added return 0 for int main()
}
