/***********************************************************************************************************************************
 * @file Mounsande_Danielou_Ubueng_4.c
 * @brief Implementiert ein zufälliges LED-Spiel, bei dem eine zufällige LED ausgewählt wird und auf den Tastendruck des Benutzers gewartet wird.
 * 
 * Diese Datei enthält ein einfaches Programm zur Steuerung von LEDs, die an einen Mikrocontroller angeschlossen sind.
 * Das Programm wählt zufällig eine der vier LEDs aus und wartet darauf, dass der Benutzer den entsprechenden Knopf drückt.
 * Die ausgewählte LED wird eingeschaltet, und nach dem Tastendruck wird die LED wieder ausgeschaltet.
 * Dieser Vorgang wiederholt sich kontinuierlich, wodurch ein zufälliges LED-Spiel entsteht.
 *
 * @autor Mounsande Danielou
 * @datum 18.11.2024
 * @version 1.0
 *************************************************************************************************************************************/

#include "main.h"

/**
 * @brief Führt das zufällige LED-Spiel aus.
 * 
 * Diese Funktion wählt eine der vier LEDs (verbunden mit den Pins `PC4`, `PC5`, `PC6`, `PC7`),
 * schaltet sie ein und wartet darauf, dass der entsprechende Knopf (der mit dem gleichen Pin verbunden ist) gedrückt wird.
 * Nachdem der Knopf gedrückt wurde, wird die LED ausgeschaltet und der Vorgang wiederholt sich.
 * 
 * Es wird die Funktion `rand()` verwendet, um eine zufällige LED auszuwählen, und es werden Entprelltechniken angewendet,
 * um eine zuverlässige Erkennung des Tastendrucks sicherzustellen.
 */
void button_by_light() {
    // Konfiguriere die Pins PC4 bis PC7 als Eingänge mit Pull-Up-Widerständen
    PORTC.DIRCLR = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    PORTC.PIN4CTRL = ~PORT_PULLUPEN_bm;
    PORTC.PIN5CTRL = ~PORT_PULLUPEN_bm;
    PORTC.PIN6CTRL = ~PORT_PULLUPEN_bm;
    PORTC.PIN7CTRL = ~PORT_PULLUPEN_bm;

    while (1) {
        uint8_t random_led = PIN4_bm << (rand() % 4); 
        PORTD.OUT = random_led;                   
        while ((PORTC.IN & random_led) != 0) {
            // Schleife bis der Knopf gedrückt wird
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
 * Diese Funktion richtet den Mikrocontroller ein, konfiguriert die LEDs und startet das zufällige LED-Spiel.
 * Sie läuft in einer Endlosschleife und ruft die Funktion `button_by_light()` auf.
 *
 * @return Diese Funktion gibt nichts zurück, da sie eine Endlosschleife ausführt.
 */

int main(void) {
    // Konfiguriere alle LEDs als Ausgänge
    PORTD.DIRSET = ALL_LEDS;

    // Starte das zufällige LED-Spiel
	button_by_light();
}

