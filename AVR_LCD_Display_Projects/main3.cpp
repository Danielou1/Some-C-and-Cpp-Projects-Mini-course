/**
 * @file main3.c
 * @brief Animation eines "Ping-Pong"-Effekts auf einem LCD-Bildschirm.
 *
 * @details
 * Dieses Programm animiert eine "1", die sich auf einem LCD-Bildschirm hin- und herbewegt. 
 * Die Animation wechselt die Zeile und Richtung, wenn der Rand des Displays erreicht wird. 
 * Der restliche Bereich der Zeilen wird mit "0" gefllt.
 *
 * @author Danielou Mounsande
 * @date 28. November 2024
 */

#include "main.h"

/**
 * @brief Fhrt eine "Ping-Pong"-Animation auf dem LCD-Bildschirm aus.
 *
 * @details
 * Die Funktion bewegt eine "1" auf einem zweizeiligen LCD von links nach rechts und 
 * umgekehrt. Wenn die "1" den Rand erreicht, wird die Zeile gewechselt und die Richtung umgekehrt. 
 * Die Animation luft endlos.
 *
 * @note Der Bildschirm wird bei jedem Zeilenwechsel gelscht.
 */
void ping_pong() {
    char line1[LCD_WIDTH + 1] = {0}; 
    char line2[LCD_WIDTH + 1] = {0};

    int position = 0;   /**< Aktuelle Position der "1". */
    int direction = 1;  /**< Richtung der Bewegung (1 = rechts, -1 = links). */
    int current_row = 0; /**< Aktuelle Zeile (0 = obere Zeile, 1 = untere Zeile). */

    while (true) { // Use true instead of 1 for C++
        // Setze die Zeilen zurck
        for (int i = 0; i < LCD_WIDTH; i++) {
            line1[i] = line2[i] = '0';
        }

        // Aktualisiere die "1" in der entsprechenden Zeile
        if (current_row == 0) {
            line1[position] = '1';
            lcd_moveCursor(0, 0);
            lcd_putString(line1);
        } else {
            line2[position] = '1';
            lcd_moveCursor(0, 1);
            lcd_putString(line2);
        }

        _delay_ms(WAIT);

        // Bewege die Position
        position += direction;

        // Wechsel der Zeile und Richtung bei Erreichen des Randes
        if (position == LCD_WIDTH || position < 0) {
            lcd_clear();
            direction = -direction;
            current_row = 1 - current_row; // Zeilenwechsel
            position += direction;
        }
    }
}

/**
 * @brief Hauptfunktion zur Initialisierung und Start der Animation.
 *
 * @return 0 bei erfolgreichem Abschluss.
 */
/*
int main() { // Changed from main() to int main()
    lcd_init();          // Initialisierung des LCD
    lcd_enable(true);    // LCD aktivieren
    lcd_backlight(true); // Hintergrundbeleuchtung einschalten

    ping_pong();         // Animation starten

    return 0;
}
*/