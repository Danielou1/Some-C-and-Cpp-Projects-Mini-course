/**
 * @file main8.c
 * @brief Implementiert einen Taster-gesteuerten Zhler mit Entprellung.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Dieses Programm zhlt bei jedem Drcken eines Tasters den Zhlerstand um eins hoch
 * und zeigt den aktuellen Stand auf einer LED-Anzeige (verbunden mit PORTD) an. Der Taster
 * (verbunden mit PIN4 von PORTC) ist gegen Prellen geschtzt.
 */

#include "main.h"

/**
 * @brief Taster-gesteuerter Zhler mit Entprellung.
 * 
 * Diese Funktion zhlt jedes Mal, wenn die zugehrige Taste (PIN4 von PORTC) gedrckt wird,
 * den Zhler um eins hoch. Der Zhler wird bei Erreichen des Wertes 256 auf 0 zurckgesetzt.
 * Die Ausgabe des Zhlerwertes erfolgt ber die LEDs, die an PORTD angeschlossen sind.
 * 
 * @details
 * - Aktiviert einen Pull-Up-Widerstand fr PIN4, um sicherzustellen, dass der Eingang 
 *   einen definierten logischen Zustand hat.
 * - berprft, ob die Taste gedrckt ist (LOW-Signal).
 * - Fhrt eine Entprellung durch, indem auf das Loslassen der Taste gewartet wird, 
 *   bevor der nchste Zhlerwert verarbeitet wird.
 * 
 * @note Diese Funktion sollte kontinuierlich in einer Endlosschleife aufgerufen werden.
 */
void debounced_counter() {
    static uint8_t counter = 0; /**< Zhlerstand (0 bis 255). */
    
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
        busy_wait(WAITING_FOR_DEBOUNCING); /**< Fhrt eine Verzgerung fr die Entprellung durch. */
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Konfiguriert alle LEDs (PORTD) als Ausgnge.
 * - Ruft die Funktion `debounced_counter` in einer Endlosschleife auf, um den Zhler zu steuern.
 * 
 * @note Entfernen Sie die Kommentare, um die Funktion zu aktivieren.
 */
/*
int main() { // Changed from main() to int main()
    PORTD.DIRSET = ALL_LEDS; 
    
    while (true) { // Use true instead of 1 for C++
        debounced_counter(); 
    }
    return 0; // Added return 0 for int main()
}
*/