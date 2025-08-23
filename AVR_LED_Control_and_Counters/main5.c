/*********************************************************************************
 * @file main5.c
 * @brief Implementiert einen Gray-Code-Zähler auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enthält die Implementierung eines Gray-Code-Zählers. Der Gray-Code
 * wird kontinuierlich hochgezählt und auf einer Reihe von LEDs dargestellt, die
 * mit den Pins des Mikrocontrollers verbunden sind.
 *********************************************************************************/

#include "main.h"

/**
 * @brief Zeigt einen Gray-Code-Zähler auf den LEDs an.
 * 
 * Der Gray-Code unterscheidet sich vom Binärzähler dadurch, dass sich nur ein Bit
 * zwischen aufeinanderfolgenden Zahlen ändert. Dies wird durch eine einfache
 * mathematische Operation erreicht: `counter ^ (counter >> 1)`. Der Zähler beginnt
 * bei 0 und zählt kontinuierlich hoch, wobei die Werte auf die LEDs geschrieben werden.
 * 
 * @details
 * - Der aktuelle Wert des Gray-Codes wird auf `PORTD.OUT` ausgegeben.
 * - Die Geschwindigkeit des Zählens kann durch den Wert von `BINARY_COUNTER_SPEED`
 *   angepasst werden.
 * - Nach dem Überlauf (255) beginnt der Zähler wieder bei 0.
 * 
 * @note Vor dem Aufruf dieser Funktion müssen alle LEDs als Ausgänge konfiguriert
 *       werden.
 */
void gray_code() {
    uint8_t counter = 0; 

    while (1) {
        PORTD.OUT = counter ^ (counter >> 1); /**< Berechnet und zeigt den Gray-Code-Wert. */
        busy_wait(BINARY_COUNTER_SPEED); 
        counter++; 
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Initialisiert die LEDs als Ausgänge.
 * - Führt die Funktion `gray_code()` in einer Endlosschleife aus, um den
 *   Gray-Code-Zähler auf den LEDs darzustellen.
 * 
 * @note Entfernen Sie die Kommentare um `main`, um das Programm auszuführen.
 * 
 * @return Gibt nichts zurück, da die Funktion unendlich läuft.
 */
/*
main() {
    PORTD.DIRSET = ALL_LEDS; // Konfiguriert alle LEDs als Ausgänge.

    while (1) {
        gray_code(); // Startet den Gray-Code-Zähler.
    }
}
*/