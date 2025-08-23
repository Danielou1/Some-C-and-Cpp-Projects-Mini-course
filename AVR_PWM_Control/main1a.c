/**************************************************************************************** 
 * @file main1a.c
 * @brief Konfiguration und Nutzung des Timers TCA0 zur PWM-Ausgabe auf dem AVR128DB48.
 *
 * Dieses Programm konfiguriert den Timer TCA0 im Single-Slope PWM-Modus, 
 * um ein PWM-Signal auf Pin PD0 über PORTMUX zu erzeugen. 
 * Die Frequenz und das Tastverhältnis werden über die TCA0-Register eingestellt.
 *
 * @author Danielou Mounsande
 * @date 12.12.2024
 ******************************************************************************************/

#include "main.h" /**< Einbinden der Haupt-Headerdatei */

/**
 * @brief Hauptfunktion des Programms.
 *
 * Initialisiert und konfiguriert den Timer TCA0, um ein PWM-Signal auf Pin PD0 zu erzeugen. 
 * Anschließend bleibt das Programm in einer Endlosschleife.
 *
 * @return Immer 0 (nicht verwendet in dieser Implementierung).
 */
/*
int main() {
    TCA0.SINGLE.PER = 100;
    TCA0.SINGLE.CMP0 = DEFAULT_PERCENT;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL2_bm; // Teiler durch 4
    PORTMUX_TCAROUTEA = PORTMUX_TCA0_PORTD_gc;
    PORTD.DIRSET = PIN0_bm;
    while (1) {
        // Endlosschleife: Die PWM-Steuerung erfolgt durch den Timer TCA0.
    }
}
*/