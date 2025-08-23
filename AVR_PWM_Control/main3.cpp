/**
 * @file main.c
 * @brief Steuerung eines Servomotors mittels PWM-Signal ber den Timer TCA0.
 *
 * Dieses Programm verwendet den AVR128DB48-Mikrocontroller, um einen Servomotor 
 * mithilfe eines PWM-Signals auf Port PF4 zu steuern. Der Motor bewegt sich 
 * zwischen den linken, mittleren und rechten Positionen mit sanfter Verzgerung.
 *
 * @author Danielou Mounsande
 * @date 12.12.2024
 */

#include "main.h" /**< Einbinden der Haupt-Headerdatei */

/**
 * @brief Konfiguration des PWM-Signals fr den Timer TCA0 auf Pin PF4.
 *
 * Diese Funktion konfiguriert den Timer TCA0 im Single-Slope PWM-Modus und setzt 
 * die PWM-Periode auf 20 ms. Die PWM-Ausgabe wird auf Pin PF4 umgeleitet.
 */
void configurePWM() {
    PORTF.DIRSET = PIN4_bm; 
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTF_gc;
    TCA0.SINGLE.PER = PERIOD_CYCLES;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc | TCA_SINGLE_ENABLE_bm;
}

/**
 * @brief Einstellen der Servomotor-Position durch Pulsbreite.
 *
 * Diese Funktion setzt die Pulsbreite fr das PWM-Signal des Servos und begrenzt 
 * die Werte zwischen der linken und rechten Extremposition.
 *
 * @param pulseWidth Die gewnschte Pulsbreite in Zyklen (zwischen SERVO_LEFT und SERVO_RIGHT).
 */
void setServoPosition(uint16_t pulseWidth) {
    if (pulseWidth < SERVO_LEFT) pulseWidth = SERVO_LEFT; 
    if (pulseWidth > SERVO_RIGHT) pulseWidth = SERVO_RIGHT;

    /** 
     * Verwendung des CMP0 Puffers fr eine synchronisierte PWM-Aktualisierung.
     */
    TCA0.SINGLE.CMP0BUF = pulseWidth;
}

/**
 * @brief Hauptfunktion zur Steuerung des Servomotors.
 *
 * Diese Funktion initialisiert die PWM-Konfiguration und bewegt den Servomotor 
 * zwischen der linken und rechten Position mit sanften bergnge und kurzen Pausen.
 *
 * @return Gibt 0 zurck (nicht verwendet in diesem Programm).
 */

int main() {
    int8_t step = 5; 
    uint16_t pulseWidth = SERVO_LEFT; 
    configurePWM(); 
    _delay_ms(1000); 

    while (true) { // Use true instead of 1 for C++
        setServoPosition(pulseWidth);
        _delay_ms(500);
        pulseWidth = static_cast<uint16_t>(pulseWidth + step); // Cast to uint16_t
        if (pulseWidth <= SERVO_LEFT || pulseWidth >= SERVO_RIGHT) {
            step = static_cast<int8_t>(-step); // Cast to int8_t
            _delay_ms(1000); 
        }
    }
    return 0; // Added return 0 for int main()
}
