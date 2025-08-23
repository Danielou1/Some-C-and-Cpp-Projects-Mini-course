/**
 * @file main8.c
 * @brief Implementiert einen Taster-gesteuerten Zähler mit Entprellung.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Dieses Programm zählt bei jedem Drücken eines Tasters den Zählerstand um eins hoch
 * und zeigt den aktuellen Stand auf einer LED-Anzeige (verbunden mit PORTD) an. Der Taster
 * (verbunden mit PIN4 von PORTC) ist gegen Prellen geschützt.
 */

#include "main.h"

/**
 * @brief Taster-gesteuerter Zähler mit Entprellung.
 * 
 * Diese Funktion zählt jedes Mal, wenn die zugehörige Taste (PIN4 von PORTC) gedrückt wird,
 * den Zähler um eins hoch. Der Zähler wird bei Erreichen des Wertes 256 auf 0 zurückgesetzt.
 * Die Ausgabe des Zählerwertes erfolgt über die LEDs, die an PORTD angeschlossen sind.
 * 
 * @details
 * - Aktiviert einen Pull-Up-Widerstand für PIN4, um sicherzustellen, dass der Eingang 
 *   einen definierten logischen Zustand hat.
 * - Überprüft, ob die Taste gedrückt ist (LOW-Signal).
 * - Führt eine Entprellung durch, indem auf das Loslassen der Taste gewartet wird, 
 *   bevor der nächste Zählerwert verarbeitet wird.
 * 
 * @note Diese Funktion sollte kontinuierlich in einer Endlosschleife aufgerufen werden.
 */
void debounced_counter() {
    static uint8_t counter = 0; /**< Zählerstand (0 bis 255). */
    
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
        busy_wait(WAITING_FOR_DEBOUNCING); /**< Führt eine Verzögerung für die Entprellung durch. */
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Konfiguriert alle LEDs (PORTD) als Ausgänge.
 * - Ruft die Funktion `debounced_counter` in einer Endlosschleife auf, um den Zähler zu steuern.
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