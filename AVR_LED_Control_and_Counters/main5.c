/*********************************************************************************
 * @file main5.c
 * @brief Implementiert einen Gray-Code-Z�hler auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enth�lt die Implementierung eines Gray-Code-Z�hlers. Der Gray-Code
 * wird kontinuierlich hochgez�hlt und auf einer Reihe von LEDs dargestellt, die
 * mit den Pins des Mikrocontrollers verbunden sind.
 *********************************************************************************/

#include "main.h"

/**
 * @brief Zeigt einen Gray-Code-Z�hler auf den LEDs an.
 * 
 * Der Gray-Code unterscheidet sich vom Bin�rz�hler dadurch, dass sich nur ein Bit
 * zwischen aufeinanderfolgenden Zahlen �ndert. Dies wird durch eine einfache
 * mathematische Operation erreicht: `counter ^ (counter >> 1)`. Der Z�hler beginnt
 * bei 0 und z�hlt kontinuierlich hoch, wobei die Werte auf die LEDs geschrieben werden.
 * 
 * @details
 * - Der aktuelle Wert des Gray-Codes wird auf `PORTD.OUT` ausgegeben.
 * - Die Geschwindigkeit des Z�hlens kann durch den Wert von `BINARY_COUNTER_SPEED`
 *   angepasst werden.
 * - Nach dem �berlauf (255) beginnt der Z�hler wieder bei 0.
 * 
 * @note Vor dem Aufruf dieser Funktion m�ssen alle LEDs als Ausg�nge konfiguriert
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
 * - Initialisiert die LEDs als Ausg�nge.
 * - F�hrt die Funktion `gray_code()` in einer Endlosschleife aus, um den
 *   Gray-Code-Z�hler auf den LEDs darzustellen.
 * 
 * @note Entfernen Sie die Kommentare um `main`, um das Programm auszuf�hren.
 * 
 * @return Gibt nichts zur�ck, da die Funktion unendlich l�uft.
 */
/*
main() {
    PORTD.DIRSET = ALL_LEDS; // Konfiguriert alle LEDs als Ausg�nge.

    while (1) {
        gray_code(); // Startet den Gray-Code-Z�hler.
    }
}
*/