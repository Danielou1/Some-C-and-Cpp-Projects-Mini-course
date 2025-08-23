/**
 * @file main2.c
 * @brief Konvertierung von Ganzzahlen und Anzeige auf einem LCD-Bildschirm.
 *
 * @details
 * Das Programm konvertiert Ganzzahlen in Zeichenketten und zeigt sie auf einem LCD an. 
 * Es verwendet Bitoperationen, um negative Zahlen im Zweierkomplement darzustellen, 
 * und beinhaltet eine Funktion, die von 0 bis 1000 zählt und die Werte schrittweise anzeigt.
 *
 * @author Danielou Mounsande
 * @date 28. November 2024
 */

#include "main.h"

/**
 * @brief Konvertiert eine Ganzzahl in eine Zeichenkette.
 *
 * @param buf Puffer zur Speicherung der Zeichenkette.
 * @param num Die zu konvertierende Ganzzahl.
 * @param base Das Zahlensystem (z. B. 10 für Dezimal, 2 für Binär).
 * @return Ein Zeiger auf den resultierenden Puffer.
 *
 * @details
 * Unterstützt die Konvertierung von negativen Zahlen, einschließlich der Darstellung 
 * im Zweierkomplement, wenn `base == 2` ist. Die Ziffern werden in die richtige Reihenfolge 
 * gebracht und als nullterminierte Zeichenkette zurückgegeben.
 */
extern char* integer_to_string(char *buf, int32_t num, int base) {
    bool isNegative = false;
    int digitCounter = 0;

    if (num == 0) {
        buf[digitCounter++] = '0';
    }

    if (num < 0) {
        isNegative = true;
        num = ~num + 1;  // Zweierkomplement
    }

    while (num > 0) {
        buf[digitCounter++] = '0' + num % base;
        num /= base;
    }

    if (isNegative) {
        buf[digitCounter++] = '-';
    }

    buf[digitCounter] = '\0';

    // Umkehren der Zeichenkette
    for (int i = 0, j = digitCounter - 1; i < j; i++, j--) {
        char temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
    }

    return buf;
}

/**
 * @brief Zählt von 0 bis 1000 und zeigt die Werte auf dem LCD an.
 *
 * @details
 * Diese Funktion löscht das LCD nach jedem Schritt und zeigt die aktuelle Zahl 
 * in Dezimalform an. Zwischen den Schritten gibt es eine definierte Verzögerung.
 */
void count_up() {
    char buf[12];

    for (int32_t counter = 0; counter <= 1000; counter++) {
        lcd_clear();
        lcd_putString(integer_to_string(buf, counter, 10));
        lcd_moveCursor(0, 0);
        _delay_ms(WAIT);
    }
}

/**
 * @brief Hauptfunktion des Programms.
 *
 * @details
 * Initialisiert das LCD, aktiviert die Hintergrundbeleuchtung und startet die 
 * Zählerfunktion `count_up`.
 *
 * @return 0 bei erfolgreichem Abschluss.
 */
/*
int main() {
    lcd_init();
    lcd_enable(true);
    lcd_backlight(true);

    count_up();
    return 0;
}
*/
