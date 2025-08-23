/***************************************************************************
 * @file main3.c
 * @brief Beschreibung der Hauptdatei.
 * 
 * Diese Datei enthält den Hauptcode für das Klavierprojekt.
 * 
 * @autor Danielou Mounsande
 * @datum 2025-01-17
 *******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "song.h"

#define F_CPU (4000000UL)
#include <util/delay.h>
#define SINE_VALUE 64
#define TABLE_SIZE 64
#define TCA_PER(x) ((uint16_t)(F_CPU / (x * SINE_VALUE)))
#define NOTE_DURATION_MS(bpm, length) ((60000UL / (bpm)) * (length))

/**
 * @brief Index der Sinustabelle.
 */
volatile uint8_t sine_index = 0;

/**
 * @brief Aktueller Index der Note.
 */
volatile uint8_t note_index = 0;

/**
 * @brief Flag, das anzeigt, ob die Note beendet ist.
 */
volatile uint8_t note_done = 0;

/**
 * @brief Dauer der aktuellen Note in Millisekunden.
 */
volatile uint16_t note_duration_ms = 0;

/**
 * @brief Verstrichene Zeit in Millisekunden.
 */
volatile uint16_t elapsed_time_ms = 0;

/**
 * @brief Initialisiert den DAC.
 */
void initialize_dac(void);

/**
 * @brief Setzt die Frequenz der Note.
 * 
 * @param frequency Die Frequenz der Note.
 */
void set_frequency(uint16_t frequency);

/**
 * @brief Spielt eine Melodie.
 * 
 * @param melody Zeiger auf die Melodie, die gespielt werden soll.
 */
void play_melody(const song *melody);

/**
 * @brief Initialisiert die Timer.
 */
void initialize_timers(void);

/**
 * @brief ISR für den Timerüberlauf zur Sinuswellengenerierung.
 */
ISR(TCA0_OVF_vect);

/**
 * @brief ISR für den Timerüberlauf zur Notendauer (1 ms Auflösung).
 */
ISR(TCA1_OVF_vect);

void initialize_dac() {
    VREF.DAC0REF = VREF_REFSEL_2V048_gc;
    DAC0.CTRLA = DAC_ENABLE_bm | DAC_OUTEN_bm; // DAC und Ausgang aktivieren
}

void set_frequency(uint16_t frequency) {
    if (frequency == mute) {
        TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm; // Ton ausschalten durch Deaktivieren des Timers
    } else {
        TCA0.SINGLE.PER = TCA_PER(frequency);      // Timerperiode für die Frequenz einstellen
        TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; // Sicherstellen, dass der Timer aktiviert ist
    }
}

void play_melody(const song *melody) {
    for (note_index = 0; note_index < melody->length; note_index++) {
        uint16_t frequency = melody->tone[note_index];
        uint8_t duration = melody->tone_length[note_index];
        note_duration_ms = NOTE_DURATION_MS(melody->bpm, duration);

        set_frequency(frequency);

        note_done = 0;
        while (!note_done); // Warten, bis die Notendauer abgelaufen ist

        set_frequency(mute); // Kurze Pause zwischen den Noten
        _delay_ms(50);
    }
}

void initialize_timers() {
    // Timer für die Frequenzerzeugung (TCA0)
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc; // Prescaler auf 64 einstellen
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;       // Überlauf-Interrupt aktivieren

    // Timer für die Notendauer (TCA1)
    TCA1.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;
    TCA1.SINGLE.PER = F_CPU / 64 / 1000;            // Auflösung auf 1 ms einstellen
    TCA1.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
}

ISR(TCA0_OVF_vect) {
    DAC0.DATA = sine_table[sine_index++] << 6;  // Ausgabe des Sinuswellenwerts
    sine_index %= TABLE_SIZE;                   // Schleife durch die Sinustabelle
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;   // Interrupt-Flag löschen
}

ISR(TCA1_OVF_vect) {
    elapsed_time_ms++;
    if (elapsed_time_ms >= note_duration_ms) {
        note_done = 1;
        elapsed_time_ms = 0;
    }
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;   // Interrupt-Flag löschen
}

/**
 * @brief Hauptfunktion.
 * 
 * Initialisiert den DAC und die Timer und spielt die Mario-Melodie.
 */
int main(void) {
    initialize_dac();
    initialize_timers();
    sei(); // Globale Interrupts aktivieren

    // Mario-Melodie spielen, die in "song.h" definiert ist
    play_melody(&mario);

    while (1) {
        // Die Hauptschleife kann für zusätzliche Aufgaben verwendet werden, falls erforderlich
    }
}