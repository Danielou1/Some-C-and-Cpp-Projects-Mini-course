/**
 * @file main8.c
 * @brief Realisierung eines bineren Taschenrechners ber Tastensteuerung.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Dieses Programm implementiert einen bineren Taschenrechner, der durch vier Tasten gesteuert wird:
 * - **Increment (Erhhen):** Erhht den Zhler um 1.
 * - **Decrement (Verringern):** Verringert den Zhler um 1.
 * - **Linksshift:** Verschiebt den Zhlerwert um 1 Bit nach links.
 * - **Rechtsshift:** Verschiebt den Zhlerwert um 1 Bit nach rechts.
 * 
 * Der Zhlerwert wird auf einer LED-Anzeige (verbunden mit PORTD) dargestellt.
 */

#include "main.h"

/**
 * @brief Binerer Taschenrechner gesteuert durch vier Tasten.
 * 
 * @details
 * Diese Funktion erlaubt es, einen 8-Bit-Zhler basierend auf Tasteneingaben zu manipulieren.
 * Die Eingaben erfolgen ber Tasten, die mit den Pins PC4 bis PC7 verbunden sind:
 * - **PC4:** Zhler um 1 erhhen(ADDITION).
 * - **PC5:** Zhler um 1 verringern(Substraktion.
 * - **PC6:** Linksshift (Division)(<<).
 * - **PC7:** Rechtsshift(Multiplikation) (>>).
 * 
 * Ein Entprellungsmechanismus wird implementiert, um sicherzustellen, dass jede Eingabe eindeutig registriert wird.
 * 
 * @note
 * Die Funktion nutzt einen statischen Speicher, um den letzten Tasterzustand zu verfolgen und mehrfaches Auslsen zu verhindern.
 */
void binary_calculator() {
    static uint8_t counter = 0; /**< 8-Bit-Zhler, der manipuliert wird. */
    static uint8_t last_button = 0; /**< Speichert den zuletzt gedrckten Taster, um mehrfaches Auslsen zu vermeiden. */
    
    PORTC.DIRCLR = PIN4_BIS_7; /**< Konfiguriert die Pins PC4 bis PC7 als Eingnge. */
    PORTC.PIN4CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN5CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN6CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN7CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t

    // Tastenlogik
    if ((PORTC.IN & PIN4_bm) == 0 && last_button == 0) {
        busy_wait(WAITING_FOR_DEBOUNCING);
        if ((PORTC.IN & PIN4_bm) == 0) {
            counter++; /**< Zhler erhhen. */
            PORTD.OUT = counter; /**< Zhlerwert auf den LEDs anzeigen. */
            last_button = PIN4_bm; /**< Letzten Taster speichern. */
        }
    } else if ((PORTC.IN & PIN5_bm) == 0 && last_button == 0) {
        busy_wait(WAITING_FOR_DEBOUNCING);
        if ((PORTC.IN & PIN5_bm) == 0) {
            counter--; /**< Zhler verringern. */
            PORTD.OUT = counter;
            last_button = PIN5_bm;
        }
    } else if ((PORTC.IN & PIN6_bm) == 0 && last_button == 0) {
        busy_wait(WAITING_FOR_DEBOUNCING);
        if ((PORTC.IN & PIN6_bm) == 0) {
            counter <<= 1; /**< Linksshift (<<). */
            PORTD.OUT = counter;
            last_button = PIN6_bm;
        }
    } else if ((PORTC.IN & PIN7_bm) == 0 && last_button == 0) {
        busy_wait(WAITING_FOR_DEBOUNCING);
        if ((PORTC.IN & PIN7_bm) == 0) {
            counter >>= 1; /**< Rechtsshift (>>). */
            PORTD.OUT = counter;
            last_button = PIN7_bm;
        }
    }

    // Zurcksetzen des Tastenstatus, wenn keine Taste gedrckt wird
    if ((PORTC.IN & (PIN4_BIS_7)) == (PIN4_BIS_7)) {
        last_button = 0;
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Konfiguriert alle LEDs (PORTD) als Ausgnge.
 * - Ruft die Funktion `binary_calculator` in einer Endlosschleife auf.
 * 
 * @note Entfernen Sie die Kommentare, um die Funktion zu aktivieren.
 */

/*int main() { // Changed from main() to int main()
    PORTD.DIRSET = ALL_LEDS; //< Konfiguriert PORTD als Ausgabe fr die LEDs. 
    
    while (true) { // Use true instead of 1 for C++
        binary_calculator(); //< Fhrt den bineren Taschenrechner aus. 
    }
    return 0; // Added return 0 for int main()
}*/
