/**
 * @file main8.c
 * @brief Implementiert einen Taster-gesteuerten Z�hler mit Entprellung.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Dieses Programm z�hlt bei jedem Dr�cken eines Tasters den Z�hlerstand um eins hoch
 * und zeigt den aktuellen Stand auf einer LED-Anzeige (verbunden mit PORTD) an. Der Taster
 * (verbunden mit PIN4 von PORTC) ist gegen Prellen gesch�tzt.
 */

#include "main.h"

/**
 * @brief Taster-gesteuerter Z�hler mit Entprellung.
 * 
 * Diese Funktion z�hlt jedes Mal, wenn die zugeh�rige Taste (PIN4 von PORTC) gedr�ckt wird,
 * den Z�hler um eins hoch. Der Z�hler wird bei Erreichen des Wertes 256 auf 0 zur�ckgesetzt.
 * Die Ausgabe des Z�hlerwertes erfolgt �ber die LEDs, die an PORTD angeschlossen sind.
 * 
 * @details
 * - Aktiviert einen Pull-Up-Widerstand f�r PIN4, um sicherzustellen, dass der Eingang 
 *   einen definierten logischen Zustand hat.
 * - �berpr�ft, ob die Taste gedr�ckt ist (LOW-Signal).
 * - F�hrt eine Entprellung durch, indem auf das Loslassen der Taste gewartet wird, 
 *   bevor der n�chste Z�hlerwert verarbeitet wird.
 * 
 * @note Diese Funktion sollte kontinuierlich in einer Endlosschleife aufgerufen werden.
 */
void debounced_counter() {
    static uint8_t counter = 0; /**< Z�hlerstand (0 bis 255). */
    
    PORTC.DIRCLR = PIN4_bm; 
    PORTC.PIN4CTRL |= PORT_PULLUPEN_bm; 

    if ((PORTC.IN & PIN4_bm) == 0) { 
        counter++; 
        if (counter == 256) {
            counter = 0; 
        }
        PORTD.OUT = counter; 
        
        while ((PORTC.IN & PIN4_bm) == 0) {
            /**< Wartet, bis die Taste losgelassen wird. */
        }
        busy_wait(WAITING_FOR_DEBOUNCING); /**< F�hrt eine Verz�gerung f�r die Entprellung durch. */
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Konfiguriert alle LEDs (PORTD) als Ausg�nge.
 * - Ruft die Funktion `debounced_counter` in einer Endlosschleife auf, um den Z�hler zu steuern.
 * 
 * @note Entfernen Sie die Kommentare, um die Funktion zu aktivieren.
 */
/*
main() {
    PORTD.DIRSET = ALL_LEDS; 
    
    while (1) {
        debounced_counter(); 
    }
}
*/