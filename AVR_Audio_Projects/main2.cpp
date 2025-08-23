#include <avr/io.h>
#include <avr/interrupt.h>
// #include <stdio.h> // Not strictly needed for this C++ conversion unless printf is used
#include "song.h"

#define F_CPU 4000000UL

#define TCA_PER(x) static_cast<uint16_t>(F_CPU / (static_cast<float>(x) * 64) - 1) // Cast to float for division

#define BUTTON_PINS (PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)

/**
 * @brief Globale Variable fr die Zeit in Millisekunden.
 */
volatile uint16_t millis = 0;

/**
 * @brief Aktuell gespielte Note.
 */
volatile uint8_t current_note = mute;

/**
 * @brief Aktueller Index in der Sinustabelle.
 */
volatile uint8_t sine_index = 0;

/**
 * @brief Initialisiert den DAC.
 */
void init_dac(); // Removed void from parameter list for C++

/**
 * @brief ISR fr die Tasten am Port B.
 */
ISR(PORTB_PORT_vect) {
    if (PORTB.IN & PIN0_bm) {
        current_note = a;
        TCA0.SINGLE.PER = TCA_PER(a);
        TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
    } else {
        current_note = mute;
        TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
    }
    PORTB.INTFLAGS = PIN0_bm;
}

/**
 * @brief ISR fr die Tasten am Port A.
 */
ISR(PORTA_PORT_vect) {
    if (PORTA.IN & PIN2_bm) {
        current_note = c;
        TCA0.SINGLE.PER = TCA_PER(c);
        TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
    } else if (PORTA.IN & PIN3_bm) {
        current_note = d;
        TCA0.SINGLE.PER = TCA_PER(d);
        TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
    } else if (PORTA.IN & PIN4_bm) {
        current_note = e;
        TCA0.SINGLE.PER = TCA_PER(e);
        TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
    } else if (PORTA.IN & PIN5_bm) {
        current_note = f;
        TCA0.SINGLE.PER = TCA_PER(f);
        TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
    } else if (PORTA.IN & PIN6_bm) {
        current_note = g;
        TCA0.SINGLE.PER = TCA_PER(g);
        TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
    } else if (PORTA.IN & PIN7_bm) {
        current_note = h;
        TCA0.SINGLE.PER = TCA_PER(h);
        TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
    }
    } else {
        current_note = mute;
        TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
        DAC0.CTRLA &= ~DAC_ENABLE_bm;
    }
    PORTA.INTFLAGS = BUTTON_PINS;
}

/**
 * @brief ISR fr den Timer nach einem berlauf des Zhlers.
 */
ISR(TCA0_OVF_vect) {
    if (current_note != mute) {
        DAC0.CTRLA |= DAC_ENABLE_bm;  // DAC aktivieren
        DAC0.DATA = static_cast<uint16_t>(sine_table[sine_index] << 6); // Cast to uint16_t
        sine_index = (sine_index + 1) % 64;
    } else {
        DAC0.CTRLA &= ~DAC_ENABLE_bm;  // DAC deaktivieren
    }
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

/**
 * @brief Hauptfunktion.
 * 
 * Konfiguriert die Tasten als Eingnge und initialisiert den DAC.
 * Die Hauptschleife bleibt leer, die Interrupts behandeln die Tastendrcke.
 */
int main() { // Changed from main(void) to int main()
    // Konfiguration der Tasten als Eingnge (Klaviatur)
    PORTA.DIRCLR = BUTTON_PINS;
    PORTB.DIRCLR = PIN0_bm;
    PORTA.PIN2CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN3CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN5CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN6CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN7CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTB.PIN0CTRL = PORT_ISC_BOTHEDGES_gc;

    init_dac();

    // Timer/Counter TCA0 Konfiguration
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;
    TCA0.SINGLE.PER = TCA_PER(mute);

    sei();

    while (true) { // Use true instead of 1 for C++
        // Hauptschleife bleibt leer; Interrupts behandeln die Tastendrcke
    }
    return 0; // Added return 0 for int main()
}