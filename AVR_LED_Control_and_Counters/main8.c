/**
 * @file main8.c
 * @brief Realisierung eines bin�ren Taschenrechners �ber Tastensteuerung.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Dieses Programm implementiert einen bin�ren Taschenrechner, der durch vier Tasten gesteuert wird:
 * - **Increment (Erh�hen):** Erh�ht den Z�hler um 1.
 * - **Decrement (Verringern):** Verringert den Z�hler um 1.
 * - **Linksshift:** Verschiebt den Z�hlerwert um 1 Bit nach links.
 * - **Rechtsshift:** Verschiebt den Z�hlerwert um 1 Bit nach rechts.
 * 
 * Der Z�hlerwert wird auf einer LED-Anzeige (verbunden mit PORTD) dargestellt.
 */

#include "main.h"

/**
 * @brief Bin�rer Taschenrechner gesteuert durch vier Tasten.
 * 
 * @details
 * Diese Funktion erlaubt es, einen 8-Bit-Z�hler basierend auf Tasteneingaben zu manipulieren.
 * Die Eingaben erfolgen �ber Tasten, die mit den Pins PC4 bis PC7 verbunden sind:
 * - **PC4:** Z�hler um 1 erh�hen(ADDITION).
 * - **PC5:** Z�hler um 1 verringern(Substraktion.
 * - **PC6:** Linksshift (Division)(<<).
 * - **PC7:** Rechtsshift(Multiplikation) (>>).
 * 
 * Ein Entprellungsmechanismus wird implementiert, um sicherzustellen, dass jede Eingabe eindeutig registriert wird.
 * 
 * @note
 * Die Funktion nutzt einen statischen Speicher, um den letzten Tasterzustand zu verfolgen und mehrfaches Ausl�sen zu verhindern.
 */
void binary_calculator() {
    static uint8_t counter = 0; /**< 8-Bit-Z�hler, der manipuliert wird. */
    static uint8_t last_button = 0; /**< Speichert den zuletzt gedr�ckten Taster, um mehrfaches Ausl�sen zu vermeiden. */
    
    PORTC.DIRCLR = PIN4_BIS_7; /**< Konfiguriert die Pins PC4 bis PC7 als Eing�nge. */
    PORTC.PIN4CTRL = ~PORT_PULLUPEN_bm; 
    PORTC.PIN5CTRL = ~PORT_PULLUPEN_bm; 
    PORTC.PIN6CTRL = ~PORT_PULLUPEN_bm; 
    PORTC.PIN7CTRL = ~PORT_PULLUPEN_bm; 

    // Tastenlogik
    if ((PORTC.IN & PIN4_bm) == 0 && last_button == 0) {
        busy_wait(WAITING_FOR_DEBOUNCING);
        if ((PORTC.IN & PIN4_bm) == 0) {
            counter++; /**< Z�hler erh�hen. */
            PORTD.OUT = counter; /**< Z�hlerwert auf den LEDs anzeigen. */
            last_button = PIN4_bm; /**< Letzten Taster speichern. */
        }
    } else if ((PORTC.IN & PIN5_bm) == 0 && last_button == 0) {
        busy_wait(WAITING_FOR_DEBOUNCING);
        if ((PORTC.IN & PIN5_bm) == 0) {
            counter--; /**< Z�hler verringern. */
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

    // Zur�cksetzen des Tastenstatus, wenn keine Taste gedr�ckt wird
    if ((PORTC.IN & (PIN4_BIS_7)) == (PIN4_BIS_7)) {
        last_button = 0;
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Konfiguriert alle LEDs (PORTD) als Ausg�nge.
 * - Ruft die Funktion `binary_calculator` in einer Endlosschleife auf.
 * 
 * @note Entfernen Sie die Kommentare, um die Funktion zu aktivieren.
 */

/*main() {
    PORTD.DIRSET = ALL_LEDS; //< Konfiguriert PORTD als Ausgabe f�r die LEDs. 
    
    while (1) {
        binary_calculator(); //< F�hrt den bin�ren Taschenrechner aus. 
    }
}*/
