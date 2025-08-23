/********************************************************************************
 * @file main4.c
 * @brief Implementiert einen Binerzhler auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enthlt die Implementierung eines Binerzhlers. Die Werte werden
 * sequentiell hochgezhlt und auf einer Reihe von LEDs angezeigt, die mit den
 * Pins des Mikrocontrollers verbunden sind.
 ********************************************************************************/

#include "main.h"

/**
 * @brief Zeigt einen Binerzhler auf den LEDs an.
 * 
 * Der Zhler beginnt bei 0 und zhlt kontinuierlich hoch, bis der maximale Wert
 * (255) erreicht wird. Danach beginnt er wieder bei 0. Die Werte werden in einer
 * Endlosschleife auf die LEDs ausgegeben, sodass jede LED einen Bitwert darstellt.
 * 
 * @details
 * - Der Zhlerwert wird direkt auf den Port `PORTD.OUT` geschrieben, wodurch die
 *   LEDs den Binerwert des Zhlers anzeigen.
 * - Die Geschwindigkeit des Zhlens kann durch den Wert von `BINARY_COUNTER_SPEED`
 *   angepasst werden.
 * 
 * @note Vor dem Aufruf dieser Funktion mssen alle LEDs als Ausgnge konfiguriert
 *       werden.
 */
void binary_counter() {
    uint8_t counter = 0; /**< Startwert des Zhlers. */

    while (true) { // Use true instead of 1 for C++
        PORTD.OUT = counter; /**< Ausgabe des aktuellen Zhlerwerts auf den LEDs. */
        busy_wait(BINARY_COUNTER_SPEED); /**< Wartezeit fr die Anzeige. */
        counter++; /**< Inkrementiert den Zhler (berlauf zurck zu 0). */
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Initialisiert die LEDs als Ausgnge.
 * - Fhrt die Funktion `binary_counter()` in einer Endlosschleife aus, um den
 *   Binerzhler auf den LEDs darzustellen.
 * 
 * @note Entfernen Sie die Kommentare um `main`, um das Programm auszufhren.
 * 
 * @return Gibt nichts zurck, da die Funktion unendlich luft.
 */
/*
int main() { // Changed from main() to int main()
    PORTD.DIRSET = ALL_LEDS; // Konfiguriert alle LEDs als Ausgnge.

    while (true) { // Use true instead of 1 for C++
        binary_counter(); // Startet den Binerzhler.
    }
    return 0; // Added return 0 for int main()
}
*/