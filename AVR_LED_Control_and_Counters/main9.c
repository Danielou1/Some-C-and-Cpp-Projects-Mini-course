/***********************************************************************************************************************************
 * @file Mounsande_Danielou_Ubueng_4.c
 * @brief Implementiert ein zuf�lliges LED-Spiel, bei dem eine zuf�llige LED ausgew�hlt wird und auf den Tastendruck des Benutzers gewartet wird.
 * 
 * Diese Datei enth�lt ein einfaches Programm zur Steuerung von LEDs, die an einen Mikrocontroller angeschlossen sind.
 * Das Programm w�hlt zuf�llig eine der vier LEDs aus und wartet darauf, dass der Benutzer den entsprechenden Knopf dr�ckt.
 * Die ausgew�hlte LED wird eingeschaltet, und nach dem Tastendruck wird die LED wieder ausgeschaltet.
 * Dieser Vorgang wiederholt sich kontinuierlich, wodurch ein zuf�lliges LED-Spiel entsteht.
 *
 * @autor Mounsande Danielou
 * @datum 18.11.2024
 * @version 1.0
 *************************************************************************************************************************************/

#include "main.h"

/**
 * @brief F�hrt das zuf�llige LED-Spiel aus.
 * 
 * Diese Funktion w�hlt eine der vier LEDs (verbunden mit den Pins `PC4`, `PC5`, `PC6`, `PC7`),
 * schaltet sie ein und wartet darauf, dass der entsprechende Knopf (der mit dem gleichen Pin verbunden ist) gedr�ckt wird.
 * Nachdem der Knopf gedr�ckt wurde, wird die LED ausgeschaltet und der Vorgang wiederholt sich.
 * 
 * Es wird die Funktion `rand()` verwendet, um eine zuf�llige LED auszuw�hlen, und es werden Entprelltechniken angewendet,
 * um eine zuverl�ssige Erkennung des Tastendrucks sicherzustellen.
 */
void button_by_light() {
    // Konfiguriere die Pins PC4 bis PC7 als Eing�nge mit Pull-Up-Widerst�nden
    PORTC.DIRCLR = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    PORTC.PIN4CTRL = ~PORT_PULLUPEN_bm;
    PORTC.PIN5CTRL = ~PORT_PULLUPEN_bm;
    PORTC.PIN6CTRL = ~PORT_PULLUPEN_bm;
    PORTC.PIN7CTRL = ~PORT_PULLUPEN_bm;

    while (1) {
        uint8_t random_led = PIN4_bm << (rand() % 4); 
        PORTD.OUT = random_led;                   
        while ((PORTC.IN & random_led) != 0) {
            // Schleife bis der Knopf gedr�ckt wird
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
 * Diese Funktion richtet den Mikrocontroller ein, konfiguriert die LEDs und startet das zuf�llige LED-Spiel.
 * Sie l�uft in einer Endlosschleife und ruft die Funktion `button_by_light()` auf.
 *
 * @return Diese Funktion gibt nichts zur�ck, da sie eine Endlosschleife ausf�hrt.
 */

int main(void) {
    // Konfiguriere alle LEDs als Ausg�nge
    PORTD.DIRSET = ALL_LEDS;

    // Starte das zuf�llige LED-Spiel
	button_by_light();
}

