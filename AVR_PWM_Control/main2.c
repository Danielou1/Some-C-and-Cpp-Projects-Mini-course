/***************************************************************************
 * @file main.c
 * @brief Steuerung einer RGB-LED mittels PWM-Signalen 
 *        auf einem AVR128DB48-Mikrocontroller.
 *
 * Dieses Programm verwendet den Timer TCA0, um PWM-Signale zu erzeugen, 
 * die zur Steuerung der Farben einer RGB-LED eingesetzt werden.
 * Die Farben �berg�nge schrittweise zwischen verschiedenen Farbt�nen.
 *
 * @author Danielou Mounsande
 * @date 12.12.2024
 ****************************************************************************/
#include "main.h"
uint8_t red = 100, green = 0, blue = 0;
/**
 * @brief Setzt die PWM-Werte f�r die RGB-LED.
 *
 * Diese Funktion berechnet die Vergleichswerte (CMP) f�r den Timer 
 * anhand der RGB-Werte in Prozent und der eingestellten Periode.
 *
 * @param red   Intensit�t der roten Farbe (0-100%).
 * @param green Intensit�t der gr�nen Farbe (0-100%).
 * @param blue  Intensit�t der blauen Farbe (0-100%).
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
        // �bergang zu Gelb
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            green = i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // �bergang zu 100% Gr�n
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            red = 100 - i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // �bergang zu Hellblau
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            blue = i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // �bergang zu 100% Blau
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            green = 100 - i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // �bergang zu Violett
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            red = i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // �bergang zu 100% Rot
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            blue = 100 - i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }
    }
}
*/