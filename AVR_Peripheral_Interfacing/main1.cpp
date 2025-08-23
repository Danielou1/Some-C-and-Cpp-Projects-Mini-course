/*****************************************************************************
 * @file main1.c
 * @brief Steuerung einer LED mittels Interrupt und Taster.
 * 
 * Das Programm steuert eine LED, die ber einen Taster (an Pin C4) ein- 
 * und ausgeschaltet wird. Die Zustandsnderung erfolgt ber Interrupts. 
 * Eine einfache Entprellung wird ebenfalls implementiert.
 * 
 * @author Danielou Mounsande
 * @date 02.12.2024
 *****************************************************************************/

#include "main.h"

// Globale Variablen
/** 
 * @brief Zustandsvariable fr die LED: 0 = aus, 1 = ein.
 */
volatile uint8_t state = 0; // frs Testen bitte bite ~kommentieren :)

/**
 * @brief Interrupt-Service-Routine fr den Button an Pin C4.
 * 
 * Diese Routine wird bei einer steigenden Flanke am Pin C4 ausgelst. 
 * Die LED wird basierend auf dem aktuellen Zustand ein- oder ausgeschaltet.
 * Das Interrupt-Flag wird manuell gelscht, um weitere Interrupts zuzulassen.
 */

ISR(PORTC_PORT_vect) {
    _delay_ms(WAITING_FOR_DEBOUNCING); // Entprellzeit
    state ^= 1; // Zustand umschalten
    PORTE.OUT = (state == 0) ? static_cast<uint8_t>(PORTE.OUT & ~LED_PINS) : static_cast<uint8_t>(PORTE.OUT | LED_PINS); // LED steuern
    PORTC.INTFLAGS = PIN4_bm; // Interrupt-Flag lschen
}

/**
 * @brief Hauptprogramm zur Initialisierung und Steuerung.
 * 
 * Konfiguriert die LED-Pins als Ausgang, aktiviert Pull-ups am Taster-Pin,
 * und ermglicht Interrupts fr steigende Flanken. Die Hauptschleife wartet 
 * passiv auf Interrupts.
 * 
 * @return Kehrt nicht zurck.
 */

int main() { // Changed from main(void) to int main()
    PORTE.DIRSET = LED_PINS; 
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc; // Taster konfigurieren
    sei(); // Globale Interrupts aktivieren

    while (true) { // Use true instead of 1 for C++
        // Wartet passiv auf Interrupts
    }
    return 0; // Added return 0 for int main()
}
