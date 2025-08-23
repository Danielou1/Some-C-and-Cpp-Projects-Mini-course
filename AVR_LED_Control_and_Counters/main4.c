/********************************************************************************
 * @file main4.c
 * @brief Implementiert einen Binärzähler auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enthält die Implementierung eines Binärzählers. Die Werte werden
 * sequentiell hochgezählt und auf einer Reihe von LEDs angezeigt, die mit den
 * Pins des Mikrocontrollers verbunden sind.
 ********************************************************************************/

#include "main.h"

/**
 * @brief Zeigt einen Binärzähler auf den LEDs an.
 * 
 * Der Zähler beginnt bei 0 und zählt kontinuierlich hoch, bis der maximale Wert
 * (255) erreicht wird. Danach beginnt er wieder bei 0. Die Werte werden in einer
 * Endlosschleife auf die LEDs ausgegeben, sodass jede LED einen Bitwert darstellt.
 * 
 * @details
 * - Der Zählerwert wird direkt auf den Port `PORTD.OUT` geschrieben, wodurch die
 *   LEDs den Binärwert des Zählers anzeigen.
 * - Die Geschwindigkeit des Zählens kann durch den Wert von `BINARY_COUNTER_SPEED`
 *   angepasst werden.
 * 
 * @note Vor dem Aufruf dieser Funktion müssen alle LEDs als Ausgänge konfiguriert
 *       werden.
 */
void binary_counter() {
    uint8_t counter = 0; /**< Startwert des Zählers. */

    while (1) {
        PORTD.OUT = counter; /**< Ausgabe des aktuellen Zählerwerts auf den LEDs. */
        busy_wait(BINARY_COUNTER_SPEED); /**< Wartezeit für die Anzeige. */
        counter++; /**< Inkrementiert den Zähler (Überlauf zurück zu 0). */
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Initialisiert die LEDs als Ausgänge.
 * - Führt die Funktion `binary_counter()` in einer Endlosschleife aus, um den
 *   Binärzähler auf den LEDs darzustellen.
 * 
 * @note Entfernen Sie die Kommentare um `main`, um das Programm auszuführen.
 * 
 * @return Gibt nichts zurück, da die Funktion unendlich läuft.
 */
/*
main() {
    PORTD.DIRSET = ALL_LEDS; // Konfiguriert alle LEDs als Ausgänge.

    while (1) {
        binary_counter(); // Startet den Binärzähler.
    }
}
*/