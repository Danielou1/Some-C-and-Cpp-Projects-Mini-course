/*********************************************************************************
 * @file main5.c
 * @brief Implementiert einen Gray-Code-Zhler auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enthlt die Implementierung eines Gray-Code-Zhlers. Der Gray-Code
 * wird kontinuierlich hochgezhlt und auf einer Reihe von LEDs dargestellt, die
 * mit den Pins des Mikrocontrollers verbunden sind.
 *********************************************************************************/

#include "main.h"

/**
 * @brief Zeigt einen Gray-Code-Zhler auf den LEDs an.
 * 
 * Der Gray-Code unterscheidet sich vom Binerzhler dadurch, dass sich nur ein Bit
 * zwischen aufeinanderfolgenden Zahlen ndert. Dies wird durch eine einfache
 * mathematische Operation erreicht: `counter ^ (counter >> 1)`. Der Zhler beginnt
 * bei 0 und zhlt kontinuierlich hoch, wobei die Werte auf die LEDs geschrieben werden.
 * 
 * @details
 * - Der aktuelle Wert des Gray-Codes wird auf `PORTD.OUT` ausgegeben.
 * - Die Geschwindigkeit des Zhlens kann durch den Wert von `BINARY_COUNTER_SPEED`
 *   angepasst werden.
 * - Nach dem berlauf (255) beginnt der Zhler wieder bei 0.
 * 
 * @note Vor dem Aufruf dieser Funktion mssen alle LEDs als Ausgnge konfiguriert
 *       werden.
 */
void gray_code() {
    uint8_t counter = 0; 

    while (true) { // Use true instead of 1 for C++
        PORTD.OUT = static_cast<uint8_t>(counter ^ (counter >> 1)); /**< Berechnet und zeigt den Gray-Code-Wert. */ // Cast to uint8_t
        busy_wait(BINARY_COUNTER_SPEED); 
        counter++; 
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Initialisiert die LEDs als Ausgnge.
 * - Fhrt die Funktion `gray_code()` in einer Endlosschleife aus, um den
 *   Gray-Code-Zhler auf den LEDs darzustellen.
 * 
 * @note Entfernen Sie die Kommentare um `main`, um das Programm auszufhren.
 * 
 * @return Gibt nichts zurck, da die Funktion unendlich luft.
 */
/*
int main() { // Changed from main() to int main()
    PORTD.DIRSET = ALL_LEDS; // Konfiguriert alle LEDs als Ausgnge.

    while (true) { // Use true instead of 1 for C++
        gray_code(); // Startet den Gray-Code-Zhler.
    }
    return 0; // Added return 0 for int main()
}
*/