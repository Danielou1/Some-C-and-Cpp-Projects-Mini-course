/***********************************************************************************************************************************
 * @file Mounsande_Danielou_Ubueng_4.c
 * @brief Implementiert ein zuflliges LED-Spiel, bei dem eine zufllige LED ausgewhlt wird und auf den Tastendruck des Benutzers gewartet wird.
 * 
 * Diese Datei enthlt ein einfaches Programm zur Steuerung von LEDs, die an einen Mikrocontroller angeschlossen sind.
 * Das Programm whlt zufllig eine der vier LEDs aus und wartet darauf, dass der Benutzer den entsprechenden Knopf drckt.
 * Die ausgewhlte LED wird eingeschaltet, und nach dem Tastendruck wird die LED wieder ausgeschaltet.
 * Dieser Vorgang wiederholt sich kontinuierlich, wodurch ein zuflliges LED-Spiel entsteht.
 *
 * @autor Mounsande Danielou
 * @datum 18.11.2024
 * @version 1.0
 *************************************************************************************************************************************/

#include "main.h"

/**
 * @brief Fhrt das zufllige LED-Spiel aus.
 * 
 * Diese Funktion whlt eine der vier LEDs (verbunden mit den Pins `PC4`, `PC5`, `PC6`, `PC7`),
 * schaltet sie ein und wartet darauf, dass der entsprechende Knopf (der mit dem gleichen Pin verbunden ist) gedrckt wird.
 * Nachdem der Knopf gedrckt wurde, wird die LED ausgeschaltet und der Vorgang wiederholt sich.
 * 
 * Es wird die Funktion `rand()` verwendet, um eine zufllige LED auszuwhlen, und es werden Entprelltechniken angewendet,
 * um eine zuverlssige Erkennung des Tastendrucks sicherzustellen.
 */
void button_by_light() {
    // Konfiguriere die Pins PC4 bis PC7 als Eingnge mit Pull-Up-Widerstnden
    PORTC.DIRCLR = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    PORTC.PIN4CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN5CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN6CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN7CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t

    while (true) { // Use true instead of 1 for C++
        uint8_t random_led = static_cast<uint8_t>(PIN4_bm << (rand() % 4)); // Cast to uint8_t
        PORTD.OUT = random_led;                   
        while ((PORTC.IN & random_led) != 0) {
            // Schleife bis der Knopf gedrckt wird
        }
        busy_wait(WAITING_FOR_DEBOUNCING);
        PORTD.OUTCLR = random_led;
        while ((PORTC.IN & random_led) == 0) {
            // Schleife bis der Knopf losgelassen wird
        }
        busy_wait(WAITING_FOR_DEBOUNCING);
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * Diese Funktion richtet den Mikrocontroller ein, konfiguriert die LEDs und startet das zufllige LED-Spiel.
 * Sie luft in einer Endlosschleife und ruft die Funktion `button_by_light()` auf.
 *
 * @return Diese Funktion gibt nichts zurck, da sie eine Endlosschleife ausfhrt.
 */

int main() { // Changed from main(void) to int main()
    // Konfiguriere alle LEDs als Ausgnge
    PORTD.DIRSET = ALL_LEDS;

    // Starte das zufllige LED-Spiel
    button_by_light();
    return 0; // Added return 0 for int main()
}
