/**
 * @file main8.c
 * @brief Realisierung eines binären Taschenrechners über Tastensteuerung.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Dieses Programm implementiert einen binären Taschenrechner, der durch vier Tasten gesteuert wird:
 * - **Increment (Erhöhen):** Erhöht den Zähler um 1.
 * - **Decrement (Verringern):** Verringert den Zähler um 1.
 * - **Linksshift:** Verschiebt den Zählerwert um 1 Bit nach links.
 * - **Rechtsshift:** Verschiebt den Zählerwert um 1 Bit nach rechts.
 * 
 * Der Zählerwert wird auf einer LED-Anzeige (verbunden mit PORTD) dargestellt.
 */

#include "main.h"

/**
 * @brief Binärer Taschenrechner gesteuert durch vier Tasten.
 * 
 * @details
 * Diese Funktion erlaubt es, einen 8-Bit-Zähler basierend auf Tasteneingaben zu manipulieren.
 * Die Eingaben erfolgen über Tasten, die mit den Pins PC4 bis PC7 verbunden sind:
 * - **PC4:** Zähler um 1 erhöhen(ADDITION).
 * - **PC5:** Zähler um 1 verringern(Substraktion.
 * - **PC6:** Linksshift (Division)(<<).
 * - **PC7:** Rechtsshift(Multiplikation) (>>).
 * 
 * Ein Entprellungsmechanismus wird implementiert, um sicherzustellen, dass jede Eingabe eindeutig registriert wird.
 * 
 * @note
 * Die Funktion nutzt einen statischen Speicher, um den letzten Tasterzustand zu verfolgen und mehrfaches Auslösen zu verhindern.
 */
void binary_calculator() {
    static uint8_t counter = 0; /**< 8-Bit-Zähler, der manipuliert wird. */
    static uint8_t last_button = 0; /**< Speichert den zuletzt gedrückten Taster, um mehrfaches Auslösen zu vermeiden. */
    
    PORTC.DIRCLR = PIN4_BIS_7; /**< Konfiguriert die Pins PC4 bis PC7 als Eingänge. */
    PORTC.PIN4CTRL = ~PORT_PULLUPEN_bm; 
    PORTC.PIN5CTRL = ~PORT_PULLUPEN_bm; 
    PORTC.PIN6CTRL = ~PORT_PULLUPEN_bm; 
    PORTC.PIN7CTRL = ~PORT_PULLUPEN_bm; 

    // Tastenlogik
    if ((PORTC.IN & PIN4_bm) == 0 && last_button == 0) {
        busy_wait(WAITING_FOR_DEBOUNCING);
        if ((PORTC.IN & PIN4_bm) == 0) {
            counter++; /**< Zähler erhöhen. */
            PORTD.OUT = counter; /**< Zählerwert auf den LEDs anzeigen. */
            last_button = PIN4_bm; /**< Letzten Taster speichern. */
        }
    } else if ((PORTC.IN & PIN5_bm) == 0 && last_button == 0) {
        busy_wait(WAITING_FOR_DEBOUNCING);
        if ((PORTC.IN & PIN5_bm) == 0) {
            counter--; /**< Zähler verringern. */
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

    // Zurücksetzen des Tastenstatus, wenn keine Taste gedrückt wird
    if ((PORTC.IN & (PIN4_BIS_7)) == (PIN4_BIS_7)) {
        last_button = 0;
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Konfiguriert alle LEDs (PORTD) als Ausgänge.
 * - Ruft die Funktion `binary_calculator` in einer Endlosschleife auf.
 * 
 * @note Entfernen Sie die Kommentare, um die Funktion zu aktivieren.
 */

/*main() {
    PORTD.DIRSET = ALL_LEDS; //< Konfiguriert PORTD als Ausgabe für die LEDs. 
    
    while (1) {
        binary_calculator(); //< Führt den binären Taschenrechner aus. 
    }
}*/
