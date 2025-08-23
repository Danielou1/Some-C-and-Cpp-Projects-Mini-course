/***************************************************************************
 * @file main.c
 * @brief Steuerung einer RGB-LED mittels PWM-Signalen 
 *        auf einem AVR128DB48-Mikrocontroller.
 *
 * Dieses Programm verwendet den Timer TCA0, um PWM-Signale zu erzeugen, 
 * die zur Steuerung der Farben einer RGB-LED eingesetzt werden.
 * Die Farben bergnge schrittweise zwischen verschiedenen Farbtnen.
 *
 * @author Danielou Mounsande
 * @date 12.12.2024
 ****************************************************************************/
#include "main.h"
uint8_t red = 100, green = 0, blue = 0;
/**
 * @brief Setzt die PWM-Werte fr die RGB-LED.
 *
 * Diese Funktion berechnet die Vergleichswerte (CMP) fr den Timer 
 * anhand der RGB-Werte in Prozent und der eingestellten Periode.
 *
 * @param red   Intensitt der roten Farbe (0-100%).
 * @param green Intensitt der grnen Farbe (0-100%).
 * @param blue  Intensitt der blauen Farbe (0-100%).
 */
void setColor(uint8_t red, uint8_t green, uint8_t blue) {
	TCA0.SINGLE.CMP0 = static_cast<uint16_t>((red * TCA0.SINGLE.PER) / 100);  // Cast to uint16_t
	TCA0.SINGLE.CMP1 = static_cast<uint16_t>((green * TCA0.SINGLE.PER) / 100); // Cast to uint16_t
	TCA0.SINGLE.CMP2 = static_cast<uint16_t>((blue * TCA0.SINGLE.PER) / 100);  // Cast to uint16_t
}
/*
int main() { // Changed from main(void) to int main()
	PORTE.DIRSET = RED_PIN | GREEN_PIN | BLUE_PIN;
	TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1EN_bm |
	TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

	TCA0.SINGLE.PER = 100; 
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;

	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTE_gc; 
    while (true) { // Use true instead of 1 for C++
        // bergang zu Gelb
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            green = i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // bergang zu 100% Grn
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            red = static_cast<uint8_t>(100 - i); // Cast to uint8_t
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // bergang zu Hellblau
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            blue = i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // bergang zu 100% Blau
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            green = static_cast<uint8_t>(100 - i); // Cast to uint8_t
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // bergang zu Violett
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            red = i;
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }

        // bergang zu 100% Rot
        for (uint8_t i = 0; i < TRANSITION_STEPS; i++) {
            blue = static_cast<uint8_t>(100 - i); // Cast to uint8_t
            setColor(red, green, blue);
            _delay_ms(STEP_DELAY);
        }
    }
    return 0; // Added return 0 for int main()
}
*/