/**************************************************************************************
 * @file main3.c
 * @brief Implementiert eine "laufende Licht"-Funktion auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enthlt die Implementierung einer Funktion, die LEDs wie ein
 * laufendes Licht (Moving Light) steuert. Die LEDs bewegen sich in einer
 * Richtung und kehren am Ende um.
 **************************************************************************************/

#include "main.h"

/**
 * @brief Lsst ein Licht ber die LEDs laufen.
 * 
 * Die LEDs bewegen sich von links nach rechts und umgekehrt. Dabei wird jeweils
 * nur eine LED zur Zeit aktiviert. Wenn die letzte LED in einer Richtung erreicht
 * wird, ndert sich die Bewegungsrichtung.
 * 
 * @details
 * - Die Bewegungsgeschwindigkeit kann ber `MOVING_LIGHT_SPEED` angepasst werden.
 * - Die LEDs sind in einer Endlosschleife aktiv.
 * - Die Richtung wird ber die Konstante `RICHTUNG_RECHTS` und die Position der LEDs
 *   ber eine einfache Verschiebung (`<<` oder `>>`) bestimmt.
 * 
 * @note Die LEDs mssen als Ausgnge konfiguriert werden, bevor diese Funktion
 *       verwendet wird.
 */
void moving_light() {
    uint8_t led_position = 1;  
    uint8_t direction = RICHTUNG_RECHTS; 

    while (true) { // Use true instead of 1 for C++
        PORTD.OUT = led_position;  
        busy_wait(MOVING_LIGHT_SPEED); /**< Verzgerung fr den Bewegungseffekt. */

        // Verschiebe die LED in die aktuelle Richtung
        if (direction) {
            led_position <<= 1;  
            if (led_position == 0x80) { 
                direction = RICHTUNG_LINKS; /**< Richtungswechsel am Ende. */
            }
        } else {
            led_position >>= 1;  
            if (led_position == 0x01) {
                direction = RICHTUNG_RECHTS; /**< Richtungswechsel am Anfang. */
            }
        }
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Konfiguriert alle LEDs als Ausgnge.
 * - Fhrt die Funktion `moving_light()` in einer Endlosschleife aus, um den
 *   Lauflichteffekt darzustellen.
 * 
 * @note Um diese Funktion zu aktivieren, entfernen Sie die Kommentare um `main`.
 * 
 * @return Gibt nichts zurck, da die Funktion unendlich luft.
 */
/*
int main() { // Changed from main() to int main()
    PORTD.DIRSET = ALL_LEDS;  // Konfiguriert alle LEDs als Ausgnge.
    
    while (true) { // Use true instead of 1 for C++
        moving_light();  // Startet das Lauflicht.
    }
    return 0; // Added return 0 for int main()
}
*/