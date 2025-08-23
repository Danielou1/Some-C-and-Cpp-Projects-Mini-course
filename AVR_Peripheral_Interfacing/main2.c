/***********************************************************************
 * @file main2.c
 * @brief Sekundenzähler mit LCD-Anzeige mittels Timer-Interrupts.
 * 
 * Dieses Programm zählt die vergangenen Sekunden und zeigt die aktuelle 
 * Zeit kontinuierlich auf einem angeschlossenen LCD an. Der Zähler wird 
 * durch den Overflow eines Timers (TCA0) ausgelöst.
 * 
 * @author Danielou Mounsande
 * @date 04.12.2024
 ************************************************************************/

#include "main.h"

// Globale Variablen
/**
 * @brief Puffer für die Umwandlung von Zahlen in Zeichenketten.
 */
char buf[32];

/**
 * @brief Zähler für vergangene Sekunden.
 */
uint16_t number = 0;

/**
 * @brief Konvertiert eine Ganzzahl in eine Zeichenkette.
 * 
 * Diese Funktion wandelt eine Ganzzahl (`int32_t`) in eine Zeichenkette 
 * (`char*`) um und unterstützt verschiedene Basen (z. B. Dezimal, Hexadezimal).
 * 
 * @param buf Zeiger auf den Zeichenpuffer für die Ausgabe.
 * @param num Die zu konvertierende Zahl.
 * @param base Die Basis des Zahlensystems (z. B. 10 für Dezimal).
 * @return Zeiger auf den Zeichenpuffer mit der konvertierten Zeichenkette.
 */
char* integer_to_string(char *buf, int32_t num, int base) {
    bool isNegative = false;
    int digitCounter = 0;

    if (num == 0) {
        buf[digitCounter++] = '0';
    }

    if (num < 0) {
        isNegative = true;
        num = ~num + 1; // Zweierkomplement
    }

    while (num > 0) {
        buf[digitCounter++] = '0' + num % base;
        num /= base;
    }

    if (isNegative) {
        buf[digitCounter++] = '-';
    }

    buf[digitCounter] = '\0';

    // Zeichenkette umkehren
    for (int i = 0, j = digitCounter - 1; i < j; i++, j--) {
        char temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
    }

    return buf;
}

/**
 * @brief Interrupt-Service-Routine für den Timer Overflow (TCA0).
 * 
 * Diese ISR wird ausgelöst, wenn der Timer TCA0 einen Overflow erreicht. 
 * Der Sekundenzähler wird inkrementiert und die neue Zeit auf dem LCD angezeigt.
 */
ISR(TCA0_OVF_vect) {
    number++;              
    lcd_clear();           
    lcd_putString(integer_to_string(buf, number, 10)); 
    lcd_moveCursor(0, 0);  
    // TCA0_OVF-Flag wird automatisch gelöscht
}

/**
 * @brief Hauptprogramm zur Initialisierung und Steuerung des Timers und LCDs.
 * 
 * Im Hauptprogramm wird der Timer konfiguriert, das LCD initialisiert und 
 * globale Interrupts aktiviert. Das Programm läuft in einer Endlosschleife 
 * und wartet auf Timer-Interrupts.
 * 
 * @return Kehrt nicht zurück.
 */

int main(void) {
    lcd_init(); 
    
    // Konfiguration des Timers
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm; 
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL1_bm | TCA_SINGLE_CLKSEL2_bm; 
    TCA0.SINGLE.PER = PRESCALER; 

    sei(); 

    while (1) {
        // Wartet passiv auf Timer-Interrupts
    }
}
