/**
 * @file main.c
 * @brief Demonstration der LCD-Anzeige mit einer "Hello World"-Meldung.
 *
 * @details
 * Dieses Programm initialisiert ein LCD-Display, aktiviert dessen Hintergrundbeleuchtung 
 * und zeigt die Nachricht "Hello Display" auf der zweiten Zeile des Bildschirms an. 
 * Es dient als einfache Demonstration der Verwendung des LCD-Moduls in einem Mikrocontrollerprojekt.
 *
 * @author Danielou Mounsande
 * @date 28. November 2024
 */

#include "main.h"

/**
 * @brief Zeigt die Nachricht "Hello Display" auf dem LCD an.
 *
 * @note Diese Funktion setzt voraus, dass die LCD-Bibliothek korrekt integriert und konfiguriert ist.
 */
void print_hello_world() {
    lcd_init();               /**< Initialisiert das LCD. */
    lcd_backlight(true);     
    lcd_moveCursor(0, 1);     
    lcd_leftToRight();        
    lcd_putString("Hello Display"); 
}

/**
 * @brief Hauptprogramm des Projekts.
 * 
 * @return int Rückgabewert des Programms (0 für erfolgreichen Abschluss).
 */
/*
int main(void) {
    print_hello_world();  // Zeigt die Begrüßungsnachricht an
    return 0;             
}
*/
