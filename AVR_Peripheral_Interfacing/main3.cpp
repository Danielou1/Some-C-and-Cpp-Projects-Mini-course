/*****************************************************************************
 * @file main3.c
 * @brief Steuerung einer Ampelschaltung mit Knopfdruck und Timer-Interrupts.
 * 
 * Simuliert eine Ampelschaltung mit Zustnden Rot, Gelb und Grn. Der Timer 
 * steuert den Wechsel der Ampelphasen, whrend Knfe den manuellen Wechsel 
 * zwischen Rot und Grn auslsen.
 * 
 * @author Danielou Mounsande
 * @date 02.12.2024
 *****************************************************************************/

#include "main.h"

// Globale Variablen
/** 
 * @brief Zustand der Ampel.
 * Initialisiert mit `RED` (Rot aktiv).
 */
volatile uint8_t traffic_state = RED;

/** 
 * @brief Status des Knopfdrucks.
 * Wird auf `1` gesetzt, wenn ein Knopf gedrckt wurde.
 */
volatile uint8_t button_pressed = 0;

/**
 * @brief ISR fr Knopf-Interrupts (PORTC).
 * 
 * Erkennt, welcher Knopf gedrckt wurde, und setzt entsprechend den Status, 
 * um in der Hauptschleife den Ampelzustand zu ndern.
 */

ISR(PORTC_PORT_vect) {
    _delay_ms(WAITING_FOR_DEBOUNCING);

    if (PORTC.INTFLAGS & PIN4_bm) { 
        if (traffic_state == RED) {
            button_pressed = 1;
        }
        PORTC.INTFLAGS = PIN4_bm; 
    } else if (PORTC.INTFLAGS & PIN5_bm) { 
        if (traffic_state == GREEN) {
            button_pressed = 1;
        }
        PORTC.INTFLAGS = PIN5_bm; 
    }
}

/**
 * @brief ISR fr Timer-Interrupts (TCA0).
 * 
 * Aktualisiert den Ampelzustand basierend auf dem aktuellen Zustand 
 * und steuert die LEDs.
 */
ISR(TCA0_OVF_vect) {
    switch (traffic_state) {
        case YELLOW_TO_GREEN:
            traffic_state = GREEN;
            PORTE.OUTCLR = PIN2_bm; 
            PORTE.OUTSET = PIN1_bm; 
            break;
        case YELLOW_TO_RED:
            traffic_state = RED;
            PORTE.OUTCLR = PIN2_bm; 
            PORTE.OUTSET = PIN0_bm; 
            break;
        default:
            break;
    }
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
/**
 * @brief Hauptprogramm zur Steuerung der Ampel.
 * 
 * Initialisiert LEDs, Buttons und den Timer. Verarbeitet Benutzereingaben 
 * in einer Endlosschleife.
 */

int main() { // Changed from main(void) to int main()
    PORTE.DIRSET = LED_PINS; 
    PORTE.OUTSET = PIN0_bm;  

    PORTC.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc; 
    PORTC.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc; 

    TCA0.SINGLE.PER = PRESCALER; 
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL1_bm | TCA_SINGLE_CLKSEL2_bm; 
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm; 

    sei(); 

    while (true) { // Use true instead of 1 for C++
        if (static_cast<bool>(button_pressed)) { // Cast to bool
            button_pressed = 0; 
            if (traffic_state == RED) {
                traffic_state = YELLOW_TO_GREEN; 
                PORTE.OUTCLR = PIN0_bm;         
                PORTE.OUTSET = PIN2_bm;         
            } else if (traffic_state == GREEN) {
                traffic_state = YELLOW_TO_RED;  
                PORTE.OUTCLR = PIN1_bm;         
                PORTE.OUTSET = PIN2_bm;         
            }
        }
    }
    return 0; // Added return 0 for int main()
}
