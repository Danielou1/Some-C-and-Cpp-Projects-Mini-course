/********************************************************************************
 * @file main4.c
 * @brief Implementiert einen Bin�rz�hler auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enth�lt die Implementierung eines Bin�rz�hlers. Die Werte werden
 * sequentiell hochgez�hlt und auf einer Reihe von LEDs angezeigt, die mit den
 * Pins des Mikrocontrollers verbunden sind.
 ********************************************************************************/

#include "main.h"

/**
 * @brief Zeigt einen Bin�rz�hler auf den LEDs an.
 * 
 * Der Z�hler beginnt bei 0 und z�hlt kontinuierlich hoch, bis der maximale Wert
 * (255) erreicht wird. Danach beginnt er wieder bei 0. Die Werte werden in einer
 * Endlosschleife auf die LEDs ausgegeben, sodass jede LED einen Bitwert darstellt.
 * 
 * @details
 * - Der Z�hlerwert wird direkt auf den Port `PORTD.OUT` geschrieben, wodurch die
 *   LEDs den Bin�rwert des Z�hlers anzeigen.
 * - Die Geschwindigkeit des Z�hlens kann durch den Wert von `BINARY_COUNTER_SPEED`
 *   angepasst werden.
 * 
 * @note Vor dem Aufruf dieser Funktion m�ssen alle LEDs als Ausg�nge konfiguriert
 *       werden.
 */
void binary_counter() {
    uint8_t counter = 0; /**< Startwert des Z�hlers. */

    while (1) {
        PORTD.OUT = counter; /**< Ausgabe des aktuellen Z�hlerwerts auf den LEDs. */
        busy_wait(BINARY_COUNTER_SPEED); /**< Wartezeit f�r die Anzeige. */
        counter++; /**< Inkrementiert den Z�hler (�berlauf zur�ck zu 0). */
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Initialisiert die LEDs als Ausg�nge.
 * - F�hrt die Funktion `binary_counter()` in einer Endlosschleife aus, um den
 *   Bin�rz�hler auf den LEDs darzustellen.
 * 
 * @note Entfernen Sie die Kommentare um `main`, um das Programm auszuf�hren.
 * 
 * @return Gibt nichts zur�ck, da die Funktion unendlich l�uft.
 */
/*
main() {
    PORTD.DIRSET = ALL_LEDS; // Konfiguriert alle LEDs als Ausg�nge.

    while (1) {
        binary_counter(); // Startet den Bin�rz�hler.
    }
}
*/