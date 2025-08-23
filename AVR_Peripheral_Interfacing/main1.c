/*****************************************************************************
 * @file main1.c
 * @brief Steuerung einer LED mittels Interrupt und Taster.
 * 
 * Das Programm steuert eine LED, die �ber einen Taster (an Pin C4) ein- 
 * und ausgeschaltet wird. Die Zustands�nderung erfolgt �ber Interrupts. 
 * Eine einfache Entprellung wird ebenfalls implementiert.
 * 
 * @author Danielou Mounsande
 * @date 02.12.2024
 *****************************************************************************/

#include "main.h"

// Globale Variablen
/** 
 * @brief Zustandsvariable f�r die LED: 0 = aus, 1 = ein.
 */
volatile uint8_t state = 0; // f�rs Testen bitte bite ~kommentieren :)

/**
 * @brief Interrupt-Service-Routine f�r den Button an Pin C4.
 * 
 * Diese Routine wird bei einer steigenden Flanke am Pin C4 ausgel�st. 
 * Die LED wird basierend auf dem aktuellen Zustand ein- oder ausgeschaltet.
 * Das Interrupt-Flag wird manuell gel�scht, um weitere Interrupts zuzulassen.
 */

ISR(PORTC_PORT_vect) {
    _delay_ms(WAITING_FOR_DEBOUNCING); // Entprellzeit
    state ^= 1; // Zustand umschalten
    PORTE.OUT = (state == 0) ? (PORTE.OUT & ~LED_PINS) : (PORTE.OUT | LED_PINS); // LED steuern
    PORTC.INTFLAGS = PIN4_bm; // Interrupt-Flag l�schen
}

/**
 * @brief Hauptprogramm zur Initialisierung und Steuerung.
 * 
 * Konfiguriert die LED-Pins als Ausgang, aktiviert Pull-ups am Taster-Pin,
 * und erm�glicht Interrupts f�r steigende Flanken. Die Hauptschleife wartet 
 * passiv auf Interrupts.
 * 
 * @return Kehrt nicht zur�ck.
 */

int main(void) {
    PORTE.DIRSET = LED_PINS; 
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc; // Taster konfigurieren
    sei(); // Globale Interrupts aktivieren

    while (1) {
        // Wartet passiv auf Interrupts
    }
}
