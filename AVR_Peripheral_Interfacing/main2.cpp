/***********************************************************************
 * @file main2.c
 * @brief Sekundenzhler mit LCD-Anzeige mittels Timer-Interrupts.
 * 
 * Dieses Programm zhlt die vergangenen Sekunden und zeigt die aktuelle 
 * Zeit kontinuierlich auf einem angeschlossenen LCD an. Der Zhler wird 
 * durch den Overflow eines Timers (TCA0) ausgelst.
 * 
 * @author Danielou Mounsande
 * @date 04.12.2024
 ************************************************************************/

#include "main.h"

// Globale Variablen
/**
 * @brief Puffer fr die Umwandlung von Zahlen in Zeichenketten.
 */
char buf[32];

/**
 * @brief Zhler fr vergangene Sekunden.
 */
uint16_t number = 0;

/**
 * @brief Konvertiert eine Ganzzahl in eine Zeichenkette.
 * 
 * Diese Funktion wandelt eine Ganzzahl (`int32_t`) in eine Zeichenkette 
 * (`char*`) um und untersttzt verschiedene Basen (z. B. Dezimal, Hexadezimal).
 * 
 * @param buf Zeiger auf den Zeichenpuffer fr die Ausgabe.
 * @param num Die zu konvertierende Zahl.
 * @param base Die Basis des Zahlensystems (z. B. 10 fr Dezimal).
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
        num = static_cast<int32_t>(~num + 1); // Zweierkomplement, C++ style cast
    }

    while (num > 0) {
        buf[digitCounter++] = static_cast<char>('0' + num % base); // C++ style cast
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
 * @brief Interrupt-Service-Routine fr den Timer Overflow (TCA0).
 * 
 * Diese ISR wird ausgelst, wenn der Timer TCA0 einen Overflow erreicht. 
 * Der Sekundenzhler wird inkrementiert und die neue Zeit auf dem LCD angezeigt.
 */
ISR(TCA0_OVF_vect) {
    number++;              
    lcd_clear();           
    lcd_putString(integer_to_string(buf, number, 10)); 
    lcd_moveCursor(0, 0);  
    // TCA0_OVF-Flag wird automatisch gelscht
}

/**
 * @brief Hauptprogramm zur Initialisierung und Steuerung des Timers und LCDs.
 * 
 * Im Hauptprogramm wird der Timer konfiguriert, das LCD initialisiert und 
 * globale Interrupts aktiviert. Das Programm luft in einer Endlosschleife 
 * und wartet auf Timer-Interrupts.
 * 
 * @return Kehrt nicht zurck.
 */

int main() { // Changed from main(void) to int main()
    lcd_init(); 
    
    // Konfiguration des Timers
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm; 
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL1_bm | TCA_SINGLE_CLKSEL2_bm; 
    TCA0.SINGLE.PER = PRESCALER; 

    sei(); 

    while (true) { // Use true instead of 1 for C++
        // Wartet passiv auf Timer-Interrupts
    }
    return 0; // Added return 0 for int main()
}
