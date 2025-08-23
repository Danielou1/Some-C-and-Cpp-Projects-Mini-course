/**
 * @file main4.c
 * @brief Steuerung eines Timers mit Start/Pause- und Zusatzfunktion.
 * 
 * Dieses Programm steuert einen Timer, der mit einem Button (C4) gestartet 
 * und pausiert werden kann. Ein weiterer Button (C5) fügt dem Timer 5 Sekunden 
 * hinzu. Die verbleibende Zeit wird auf einem LCD angezeigt und die RGB-LED 
 * rot anschaltet, wenn der Timer abgelaufen ist.
 * 
 * @author Danielou Mouns
 * @date 02.12.2024
 */

#include "main.h"

// Globale Variablen
/** 
 * @brief Verbleibende Zeit in Sekunden.
 */
volatile uint16_t remaining_time = 0;

/** 
 * @brief Timerstatus, `0` für pausiert, `1` für laufend.
 */
volatile uint8_t timer_running = 0; 

/** 
 * @brief Puffer für die LCD-Ausgabe.
 */
char buf[32];

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
 * @brief LCD anzeigen aktualisieren.
 * 
 * Diese Funktion löscht den LCD-Bildschirm und zeigt die verbleibende Zeit 
 * in Sekunden an.
 */
void update_lcd() {
    lcd_clear();
    lcd_putString(integer_to_string(buf, remaining_time, 10)); 
    lcd_moveCursor(0, 0);
}

/**
 * @brief ISR für den Timer-Overflow (TCA0).
 * 
 * Diese ISR wird ausgelöst, wenn der Timer überläuft. Sie verringert die 
 * verbleibende Zeit um eine Sekunde, aktualisiert das LCD und setzt die RGB-LED 
 * auf Rot, wenn die Zeit abgelaufen ist.
 */
ISR(TCA0_OVF_vect) {
    if (timer_running && remaining_time > 0) {
        remaining_time--; 
        update_lcd();

        if (remaining_time == 0) {
            PORTE.OUTCLR = PIN1_bm | PIN2_bm; 
            PORTE.OUTSET = PIN0_bm;          
            timer_running = 0;
        }else {
			PORTE.OUTCLR = LED_PINS;
		}
    }

    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; 
}

/**
 * @brief ISR für den Button-Interrupt (Start/Pause und Hinzufügen von 5 Sekunden).
 * 
 * Diese ISR prüft, welcher Knopf gedrückt wurde. Der Button an Pin C4 startet 
 * oder pausiert den Timer, während der Button an Pin C5 5 Sekunden zur verbleibenden 
 * Zeit hinzufügt und das LCD aktualisiert.
 */
ISR(PORTC_PORT_vect) {
    _delay_ms(WAITING_FOR_DEBOUNCING); 

    if (PORTC.INTFLAGS & PIN4_bm) { 
        timer_running ^= 1; 
        PORTC.INTFLAGS = PIN4_bm; 
    } else if (PORTC.INTFLAGS & PIN5_bm) { 
        remaining_time += 5; 
        update_lcd();
        PORTC.INTFLAGS = PIN5_bm; 
    }
}

/**
 * @brief Hauptprogramm zur Initialisierung des Timers und der Peripherie.
 * 
 * Im Hauptprogramm werden das LCD, die RGB-LED und die Buttons initialisiert. 
 * Der Timer wird für 1 Hz (1 Sekunde) konfiguriert, und globale Interrupts 
 * werden aktiviert. Das Programm bleibt in einer Endlosschleife und wartet auf 
 * Interrupts.
 * 
 * @return Kehrt nicht zurück.
 */
int main(void) {
    lcd_init();

    PORTE.DIRSET = LED_PINS; 
    PORTE.OUTCLR = LED_PINS; 

    PORTC.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc; 
    PORTC.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc; 

    TCA0.SINGLE.PER = PRESCALER; 
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL1_bm | TCA_SINGLE_CLKSEL2_bm; 
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm; 

    sei(); 
    update_lcd();

    while (1) {
        // Endlosschleife, keine Aktionen erforderlich
    }
}
