/***************************************************************************************************************************
 * @file main1.c
 * @brief Dieses Programm steuert eine LED an einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * In diesem Programm wird ein Pin (PD7) als Ausgang konfiguriert, um eine daran angeschlossene LED einzuschalten.
 * Die Funktion `light_up()` aktiviert die LED. 
 * Im Hauptprogramm wird die LED in einer Endlosschleife eingeschaltet.
 **************************************************************************************************************************/

#include "main.h"

/**
 * @brief Schaltet die LED, die an Pin PD7 angeschlossen ist, ein.
 * 
 * Diese Funktion setzt das entsprechende Bit fr PD7 im PORTD.OUTSET-Register.
 * Sie kann in einer Schleife aufgerufen werden, um den Zustand der LED auf "Ein" zu halten.
 */
void light_up() {
    PORTD.OUTSET = PIN7_bm; /**< Aktiviert PD7, um die LED einzuschalten. */
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * Diese Funktion konfiguriert den Pin PD7 als Ausgang 
 * und ruft anschlieend in einer Endlosschleife die Funktion `light_up()` auf.
 * 
 * @note Um diese Funktion zu aktivieren, entfernen Sie die Kommentare um die Funktion `main`.
 * 
 * @return Gibt nichts zurck, da die Funktion unendlich luft.
 */
/*
int main() { // Changed from main() to int main()
    PORTD.DIRSET = PIN7_bm;  // Konfiguriert PD7 als Ausgang
    while(true) { // Use true instead of 1 for C++
        light_up();          // Schaltet die LED ein
    }
    return 0; // Added return 0 for int main()
}
*/