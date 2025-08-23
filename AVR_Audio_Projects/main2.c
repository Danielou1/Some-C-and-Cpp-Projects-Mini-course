/********************************************************************
 * @file main2.c
 * @brief Beschreibung der Hauptdatei.
 * 
 * Diese Datei enthält den Hauptcode für das Klavierprojekt.
 * 
 * @autor Danielou Mounsande
 * @datum 2025-01-17
 **********************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "song.h"

#define F_CPU 4000000UL

#define TCA_PER(x) ((uint16_t)(F_CPU / ((x) * 64) - 1))

#define BUTTON_PINS (PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)

/**
 * @brief Globale Variable für die Zeit in Millisekunden.
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
void init_dac() {
    VREF.DAC0REF = VREF_REFSEL_2V048_gc;
    DAC0.CTRLA = DAC_OUTEN_bm | DAC_ENABLE_bm;
    DAC0.DATA = sine_table[0] << 6;  // Start mit keinem Ton (stumm)
}

/**
 * @brief ISR für die Tasten am Port B.
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
 * @brief ISR für die Tasten am Port A.
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
    } else {
        current_note = mute;
        TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
        DAC0.CTRLA &= ~DAC_ENABLE_bm;
    }
    PORTA.INTFLAGS = BUTTON_PINS;
}

/**
 * @brief ISR für den Timer nach einem Überlauf des Zählers.
 */
ISR(TCA0_OVF_vect) {
    if (current_note != mute) {
        DAC0.CTRLA |= DAC_ENABLE_bm;  // DAC aktivieren
        DAC0.DATA = sine_table[sine_index] << 6;
        sine_index = (sine_index + 1) % 64;
    } else {
        DAC0.CTRLA &= ~DAC_ENABLE_bm;  // DAC deaktivieren
    }
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

/**
 * @brief Hauptfunktion.
 * 
 * Konfiguriert die Tasten als Eingänge und initialisiert den DAC.
 * Die Hauptschleife bleibt leer, die Interrupts behandeln die Tastendrücke.
 */
int main(void) {
    // Konfiguration der Tasten als Eingänge (Klaviatur)
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

    while (1) {
        // Hauptschleife bleibt leer; Interrupts behandeln die Tastendrücke
    }
}