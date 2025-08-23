/**
 * @file main1.c
 * @brief Hauptdatei für das Klavierprojekt.
 * 
 * Diese Datei enthält den Hauptcode für das Klavierprojekt.
 * 
 * @autor Danielou Mounda
 * @datum 2025-01-17
 */

#define F_CPU 4000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "song.h"

#define SINE_TABLE_SIZE 64
#define SIGNAL_FREQUENCY a1 // Frequenz des zu generierenden Signals

/**
 * @brief Aktueller Index des Arrays sine_table.
 */
volatile uint8_t sine_wave_index = 0;

/**
 * @brief Initialisiert den DAC.
 */
void initialize_dac(void);

/**
 * @brief Initialisiert den Timer.
 */
void initialize_timer(void);

/**
 * @brief ISR für den Timerüberlauf zur Sinuswellengenerierung.
 */
ISR(TCA0_OVF_vect);

void initialize_dac() {
    VREF.DAC0REF = VREF_REFSEL_2V048_gc; // DAC max 2V
    DAC0.CTRLA = DAC_OUTEN_bm | DAC_ENABLE_bm; // DAC aktivieren
    DAC0.DATA = sine_table[0]; // Initialer Wert
}

void initialize_timer() {
    uint16_t period = (uint16_t)((F_CPU) / (SIGNAL_FREQUENCY * SINE_TABLE_SIZE));
    
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV1_gc;
    TCA0.SINGLE.PER = period;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
}

ISR(TCA0_OVF_vect) {
    // Den DAC-Wert mit dem nächsten Wert der sine_table aktualisieren
    DAC0.DATA = sine_table[sine_wave_index] << 6;
    sine_wave_index = (sine_wave_index + 1) % SINE_TABLE_SIZE; // Nächster Index und dann wieder von vorne an
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; // Interrupt-Flag deaktivieren
}

/**
 * @brief Hauptfunktion.
 * 
 * Initialisiert den DAC und die Timer und aktiviert globale Interrupts.
 */
int main(void) {
    initialize_dac(); // DAC initialisieren
    initialize_timer(); // Timer initialisieren
    
    sei(); // Globale Interrupts aktivieren
    while (1) {
        // Hauptschleife bleibt leer
    }
}