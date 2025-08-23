/**************************************************************************************
 * @file main3.c
 * @brief Implementiert eine "laufende Licht"-Funktion auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enthält die Implementierung einer Funktion, die LEDs wie ein
 * laufendes Licht (Moving Light) steuert. Die LEDs bewegen sich in einer
 * Richtung und kehren am Ende um.
 **************************************************************************************/

#include "main.h"

/**
 * @brief Lässt ein Licht über die LEDs laufen.
 * 
 * Die LEDs bewegen sich von links nach rechts und umgekehrt. Dabei wird jeweils
 * nur eine LED zur Zeit aktiviert. Wenn die letzte LED in einer Richtung erreicht
 * wird, ändert sich die Bewegungsrichtung.
 * 
 * @details
 * - Die Bewegungsgeschwindigkeit kann über `MOVING_LIGHT_SPEED` angepasst werden.
 * - Die LEDs sind in einer Endlosschleife aktiv.
 * - Die Richtung wird über die Konstante `RICHTUNG_RECHTS` und die Position der LEDs
 *   über eine einfache Verschiebung (`<<` oder `>>`) bestimmt.
 * 
 * @note Die LEDs müssen als Ausgänge konfiguriert werden, bevor diese Funktion
 *       verwendet wird.
 */
void moving_light() {
    uint8_t led_position = 1;  
    uint8_t direction = RICHTUNG_RECHTS; 

    while (1) {
        PORTD.OUT = led_position;  
        busy_wait(MOVING_LIGHT_SPEED); /**< Verzögerung für den Bewegungseffekt. */

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
 * - Konfiguriert alle LEDs als Ausgänge.
 * - Führt die Funktion `moving_light()` in einer Endlosschleife aus, um den
 *   Lauflichteffekt darzustellen.
 * 
 * @note Um diese Funktion zu aktivieren, entfernen Sie die Kommentare um `main`.
 * 
 * @return Gibt nichts zurück, da die Funktion unendlich läuft.
 */
/*
main() {
    PORTD.DIRSET = ALL_LEDS;  // Konfiguriert alle LEDs als Ausgänge.
    
    while (1) {
        moving_light();  // Startet das Lauflicht.
    }
}
*/