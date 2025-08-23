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
void initialize_dac(); // Removed void from parameter list for C++

/**
 * @brief Initialisiert den Timer.
 */
void initialize_timer(); // Removed void from parameter list for C++

/**
 * @brief ISR fr den Timerberlauf zur Sinuswellengenerierung.
 */
ISR(TCA0_OVF_vect);

void initialize_dac() {
    VREF.DAC0REF = VREF_REFSEL_2V048_gc; // DAC max 2V
    DAC0.CTRLA = DAC_OUTEN_bm | DAC_ENABLE_bm; // DAC aktivieren
    DAC0.DATA = sine_table[0]; // Initialer Wert
}

void initialize_timer() {
    uint16_t period = static_cast<uint16_t>((F_CPU) / (SIGNAL_FREQUENCY * SINE_TABLE_SIZE)); // Cast to uint16_t
    
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV1_gc;
    TCA0.SINGLE.PER = period;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
}

ISR(TCA0_OVF_vect) {
    // Den DAC-Wert mit dem nchsten Wert der sine_table aktualisieren
    DAC0.DATA = static_cast<uint16_t>(sine_table[sine_wave_index] << 6); // Cast to uint16_t
    sine_wave_index = (sine_wave_index + 1) % SINE_TABLE_SIZE; // Nchster Index und dann wieder von vorne an
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; // Interrupt-Flag deaktivieren
}

/**
 * @brief Hauptfunktion.
 * 
 * Initialisiert den DAC und die Timer und aktiviert globale Interrupts.
 */
int main() { // Changed from main(void) to int main()
    initialize_dac(); // DAC initialisieren
    initialize_timer(); // Timer initialisieren
    
    sei(); // Globale Interrupts aktivieren
    while (true) { // Use true instead of 1 for C++
        // Hauptschleife bleibt leer
    }
    return 0; // Added return 0 for int main()
}