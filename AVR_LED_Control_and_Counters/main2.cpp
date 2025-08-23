/********************************************************************************
 * @file main2.c
 * @brief Dieses Programm steuert LEDs auf einem AVR-Mikrocontroller.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * In diesem Programm werden Funktionen definiert, um LEDs an verschiedenen Pins
 * zu steuern. Die LED an Pin PD7 kann blinken (`blinky_one_led`), und zwei LEDs 
 * (PD7 und PD2) knnen abwechselnd blinken (`blinky_two_leds`).
 *********************************************************************************/

#include "main.h"

/**
 * @brief Fhrt eine Verzgerung aus, um den Ablauf zu verlangsamen.
 * 
 * @param delay Anzahl der Schleifendurchlufe zur Verzgerung.
 * 
 * @note Die Verwendung des Modifikators `volatile` verhindert Compiler-Optimierungen.
 */
extern void busy_wait(volatile unsigned long delay) {
    for (volatile uint32_t i = 0; i < delay; i++) {
        // "volatile" verhindert Optimierungen durch den Compiler
    }
}

/**
 * @brief Lsst die LED an Pin PD7 blinken.
 * 
 * Die LED an PD7 wird ein- und ausgeschaltet. Zwischen den Zustandsnderungen
 * gibt es eine Verzgerung, die mit `busy_wait()` realisiert wird.
 */
void blinky_one_led() {
    PORTD.OUTTGL = PIN7_bm; /**< Schaltet die LED an PD7 um. */
    busy_wait(F_CPU);       /**< Verzgerung. */
    PORTD.OUTCLR = PIN7_bm; /**< Schaltet die LED an PD7 aus. */
    busy_wait(F_CPU);       /**< Verzgerung. */
}

/**
 * @brief Lsst die LEDs an den Pins PD7 und PD2 abwechselnd blinken.
 * 
 * Die LED an PD7 blinkt zuerst, dann die LED an PD2. Zwischen den Zustandsnderungen
 * gibt es Verzgerungen, die mit `busy_wait()` realisiert werden.
 */
void blinky_two_leds() {
    PORTD.OUTTGL = PIN7_bm; /**< Schaltet die LED an PD7 um. */
    busy_wait(F_CPU);       /**< Verzgerung. */
    PORTD.OUTCLR = PIN7_bm; /**< Schaltet die LED an PD7 aus. */
    PORTD.OUTTGL = PIN2_bm; /**< Schaltet die LED an PD2 um. */
    busy_wait(F_CPU);       /**< Verzgerung. */
    PORTD.OUTCLR = PIN2_bm; /**< Schaltet die LED an PD2 aus. */
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * Diese Funktion konfiguriert die Pins PD7 und PD2 als Ausgnge und
 * ruft anschlieend in einer Endlosschleife die Funktionen `blinky_one_led()` oder
 * `blinky_two_leds()` auf, je nach auskommentierter Zeile.
 * 
 * @note Um diese Funktion zu aktivieren, entfernen Sie die Kommentare um die Funktion `main`.
 * 
 * @return Gibt nichts zurck, da die Funktion unendlich luft.
 */
/*
int main() { // Changed from main() to int main()
    PORTD.DIRSET = PIN7_bm | PIN2_bm;  // Konfiguriert PD7 und PD2 als Ausgnge
    while (true) { // Use true instead of 1 for C++
        //blinky_one_led();           // Lsst PD7 blinken
        blinky_two_leds();           // Lsst PD7 und PD2 abwechselnd blinken
    }
    return 0; // Added return 0 for int main()
}
*/