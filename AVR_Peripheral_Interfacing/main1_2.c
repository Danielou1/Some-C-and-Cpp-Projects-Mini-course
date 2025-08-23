/*************************************************************************************************
 * @file main1_2.c
 * @brief Steuerung einer LED mit zufälliger Farbe mittels Interrupts und Pseudozufallszahlen.
 * 
 * Dieses Programm schaltet eine LED ein oder aus, wenn ein Taster betätigt wird. 
 * Wenn die LED eingeschaltet wird, erhält sie eine zufällige Farbe basierend 
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
 * @brief Zustand für den Xorshift32-Pseudozufallszahlengenerator.
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
 * @brief Interrupt-Service-Routine für den Button an Pin C4.
 * 
 * Diese Routine schaltet die LED ein oder aus. Wenn die LED eingeschaltet wird, 
 * erhält sie eine zufällige Farbe basierend auf der Ausgabe des `xorshift32`-Generators.
 * Nach der Verarbeitung wird das Interrupt-Flag zurückgesetzt.
 */

ISR(PORTC_PORT_vect) {
    _delay_ms(WAITING_FOR_DEBOUNCING);
    if (state == 0) {
        PORTE.OUTCLR = LED_PINS; 
    } else {
        PORTE.OUTSET = xorshift32() & LED_PINS; // Zufällige Farbe setzen
    }
    state ^= 1; // Zustand umschalten
    PORTC.INTFLAGS = PIN4_bm; // Interrupt-Flag löschen
}

/**
 * @brief Hauptprogramm zur Initialisierung und Steuerung der LED.
 * 
 * Initialisiert die LED-Pins, den Taster und die Interrupts. Das Programm läuft 
 * in einer Endlosschleife und reagiert ausschließlich auf die Interrupts.
 * 
 * @return Kehrt nicht zurück.
 */

int main(void) {
    PORTE.DIRSET = LED_PINS;
    PORTC.PIN4CTRL &= ~PORT_PULLUPEN_bm; 
    PORTC.PIN4CTRL = PORT_ISC_RISING_gc;
    sei();

    while (1) {
        // Wartet passiv auf Interrupts
    }
}
