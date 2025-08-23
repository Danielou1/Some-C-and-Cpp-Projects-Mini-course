/** 
 * @file main1.c
 * @brief Konfiguration und Nutzung des Timers TCA0 zur PWM-Ausgabe auf dem AVR128DB48.
 *
 * Dieses Programm konfiguriert den Timer TCA0 im Single-Slope PWM-Modus, 
 * um ein PWM-Signal auf Pin PD0 ber PORTMUX zu erzeugen. 
 * Die Frequenz und das Tastverhltnis werden ber die TCA0-Register eingestellt.
 *
 * @author Danielou Mounsande
 * @date 12.12.2024
 */
#include "main.h"
/** 
 * @file main1.c
 * @brief Konfiguration und Nutzung des Timers TCA0 zur PWM-Ausgabe auf dem AVR128DB48.
 *
 * Dieses Programm konfiguriert den Timer TCA0 im Single-Slope PWM-Modus, 
 * um ein PWM-Signal auf Pin PD0 ber PORTMUX zu erzeugen. 
 * Die Frequenz und das Tastverhltnis werden ber die TCA0-Register eingestellt.
 *
 * @author Danielou Mounsande
 * @date 12.12.2024
 */
void set_brightness(uint8_t percentage) {
    uint16_t pwm_value = static_cast<uint16_t>((percentage * TCA0.SINGLE.PER) / 100); // Cast to uint16_t
    TCA0.SINGLE.CMP0 = pwm_value;
}
/**
 * @brief Hauptfunktion des Programms.
 *
 * Konfiguriert den Timer TCA0 zur PWM-Ausgabe auf Pin PD0. 
 * Die Periode und das Standard-Tastverhltnis werden eingestellt, und die PWM-Ausgabe 
 * bleibt aktiv in einer Endlosschleife.
 *
 * @return Immer 0 (nicht verwendet in dieser Implementierung).
 */
/*
int main() { // Changed from main(void) to int main()
    PORTD.DIRSET = PIN0_bm;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL2_bm; //div4
    TCA0.SINGLE.PER = 100;
    PORTMUX_TCAROUTEA = PORTMUX_TCA0_PORTD_gc;
    set_brightness(DEFAULT_PERCENT);

    while (true) { // Use true instead of 1 for C++
    }
    return 0; // Added return 0 for int main()
}
*/