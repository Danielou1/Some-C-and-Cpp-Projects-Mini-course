/************************************************************************************
 * @file main3.c
 * @brief Implementierung einer Tastenabfrage mit Entprellung und UART-�bertragung.
 * @author Danielou Mounsande
 * @date 2025-01-05
 *************************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define F_CPU 4000000UL 
#define BAUDR 9600 
#define DEBOUNCE_TIME 50 
#define S 16UL 
#define BUTTON_PINS (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm) 

volatile uint32_t millis_counter = 0; 
volatile uint32_t last_press_time = 0; 
volatile char button_signal; ///< Zu sendendes Signal f�r die Taste
volatile bool is_signal_ready = false; 

/**
 * @brief Sendet ein Zeichen �ber UART.
 * @param data Das zu sendende Zeichen
 */
void transmit(char data)
{
    while (!(USART3.STATUS & USART_DREIF_bm)) {}
    USART3.TXDATAL = data;
}

/**
 * @brief ISR f�r den Timer TCA0 Overflow.
 * Inkrementiert den globalen Millisekundenz�hler.
 */
ISR(TCA0_OVF_vect)
{
    millis_counter++;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; ///< L�scht das Interrupt-Flag
}

/**
 * @brief ISR f�r die Port C-Tasten.
 * Erkennt g�ltige Tastendr�cke unter Ber�cksichtigung der Entprellzeit.
 */
ISR(PORTC_PORT_vect)
{
    uint8_t flags = PORTC.INTFLAGS & BUTTON_PINS; ///< Liest die ausgel�sten Interrupt-Pins
    PORTC.INTFLAGS = flags; ///< L�scht die Interrupt-Flags

    uint32_t current_time = millis_counter;
    if (current_time - last_press_time < DEBOUNCE_TIME) {
        return; ///< Ignoriert, wenn die Entprellzeit nicht abgelaufen ist
    }
    last_press_time = current_time; ///< Aktualisiert die Zeit des letzten g�ltigen Tastendrucks

    if (flags & PIN4_bm) {
        button_signal = 'A';
        is_signal_ready = true;
    } else if (flags & PIN5_bm) {
        button_signal = 'B';
        is_signal_ready = true;
    } else if (flags & PIN6_bm) {
        button_signal = 'C';
        is_signal_ready = true;
    } else if (flags & PIN7_bm) {
        button_signal = 'D';
        is_signal_ready = true;
    }
}

/**
 * @brief Hauptprogramm.
 * Initialisiert die Hardware und steuert den Hauptprogrammfluss.
 */
int main(void)
{
    // Konfiguriert die Pins PC4 bis PC7 als Eing�nge mit Pull-Up-Widerst�nden und steigender Flanke
    PORTC.DIRCLR = BUTTON_PINS;
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;
    PORTC.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;
    PORTC.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;
    PORTC.PIN7CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;

    cli();

    // Konfiguriert den Timer TCA0 f�r eine Basiszeit von 1 ms
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc; 
    TCA0.SINGLE.PER = 1000 - 1; ///< Periode f�r 1 ms
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm; 
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; ///< Startet den Timer

    // Konfiguriert USART3 f�r die serielle Kommunikation
    USART3.BAUD = (F_CPU * 64) / (S * BAUDR); ///< Berechnung der Baudrate
    USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm; ///< Aktiviert Sender und Empf�nger
    USART3.CTRLC = USART_CHSIZE_8BIT_gc; 

    PORTB.DIRSET = PIN0_bm; 

    sei(); 

    while (1) {
        if (is_signal_ready) {
            transmit(button_signal); // Sendet das Signal �ber UART
            is_signal_ready = false; 
        }
    }
}
