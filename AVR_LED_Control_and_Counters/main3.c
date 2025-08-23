/**************************************************************************************
 * @file main3.c
 * @brief Implementiert eine "laufende Licht"-Funktion auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enth�lt die Implementierung einer Funktion, die LEDs wie ein
 * laufendes Licht (Moving Light) steuert. Die LEDs bewegen sich in einer
 * Richtung und kehren am Ende um.
 **************************************************************************************/

#include "main.h"

/**
 * @brief L�sst ein Licht �ber die LEDs laufen.
 * 
 * Die LEDs bewegen sich von links nach rechts und umgekehrt. Dabei wird jeweils
 * nur eine LED zur Zeit aktiviert. Wenn die letzte LED in einer Richtung erreicht
 * wird, �ndert sich die Bewegungsrichtung.
 * 
 * @details
 * - Die Bewegungsgeschwindigkeit kann �ber `MOVING_LIGHT_SPEED` angepasst werden.
 * - Die LEDs sind in einer Endlosschleife aktiv.
 * - Die Richtung wird �ber die Konstante `RICHTUNG_RECHTS` und die Position der LEDs
 *   �ber eine einfache Verschiebung (`<<` oder `>>`) bestimmt.
 * 
 * @note Die LEDs m�ssen als Ausg�nge konfiguriert werden, bevor diese Funktion
 *       verwendet wird.
 */
void moving_light() {
    uint8_t led_position = 1;  
    uint8_t direction = RICHTUNG_RECHTS; 

    while (1) {
        PORTD.OUT = led_position;  
        busy_wait(MOVING_LIGHT_SPEED); /**< Verz�gerung f�r den Bewegungseffekt. */

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
 * - Konfiguriert alle LEDs als Ausg�nge.
 * - F�hrt die Funktion `moving_light()` in einer Endlosschleife aus, um den
 *   Lauflichteffekt darzustellen.
 * 
 * @note Um diese Funktion zu aktivieren, entfernen Sie die Kommentare um `main`.
 * 
 * @return Gibt nichts zur�ck, da die Funktion unendlich l�uft.
 */
/*
main() {
    PORTD.DIRSET = ALL_LEDS;  // Konfiguriert alle LEDs als Ausg�nge.
    
    while (1) {
        moving_light();  // Startet das Lauflicht.
    }
}
*/