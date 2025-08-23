/***************************************************************************
 * @file main.c
 * @brief Steuerung einer RGB-LED mittels PWM-Signalen 
 *        auf einem AVR128DB48-Mikrocontroller.
 *
 * Dieses Programm verwendet den Timer TCA0, um PWM-Signale zu erzeugen, 
 * die zur Steuerung der Farben einer RGB-LED eingesetzt werden.
 * Die Farben übergänge schrittweise zwischen verschiedenen Farbtönen.
 *
 * @author Danielou Mounsande
 * @date 12.12.2024
 ****************************************************************************/
#include "main.h"
uint8_t red = 100, green = 0, blue = 0;
/**
 * @brief Setzt die PWM-Werte für die RGB-LED.
 *
 * Diese Funktion berechnet die Vergleichswerte (CMP) für den Timer 
 * anhand der RGB-Werte in Prozent und der eingestellten Periode.
 *
 * @param red   Intensität der roten Farbe (0-100%).
 * @param green Intensität der grünen Farbe (0-100%).
 * @param blue  Intensität der blauen Farbe (0-100%).
 */
void setColor(uint8_t red, uint8_t green, uint8_t blue) {
	TCA0.SINGLE.CMP0 = (red * TCA0.SINGLE.PER) / 100;  
	TCA0.SINGLE.CMP1 = (green * TCA0.SINGLE.PER) / 100; 
	TCA0.SINGLE.CMP2 = (blue * TCA0.SINGLE.PER) / 100;  
}
/*
int main(void) {
	PORTE.DIRSET = RED_PIN | GREEN_PIN | BLUE_PIN;
	TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1EN_bm |
	TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

	TCA0.SINGLE.PER = 100; 
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;

	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTE_gc; 
    while (1) {
        // Übergang zu Gelb
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            green = i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // Übergang zu 100% Grün
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            red = 100 - i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // Übergang zu Hellblau
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            blue = i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // Übergang zu 100% Blau
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            green = 100 - i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // Übergang zu Violett
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            red = i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // Übergang zu 100% Rot
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            blue = 100 - i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }
    }
}
*/