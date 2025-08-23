#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h> // Keep for bool type if not using C++ <cstdbool>

#define F_CPU 4000000UL 
#define BAUDR 9600 
#define DEBOUNCE_TIME 50 
#define S 16UL 
#define BUTTON_PINS (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm) 

volatile uint32_t millis_counter = 0; 
volatile uint32_t last_press_time = 0; 
volatile char button_signal; ///< Zu sendendes Signal fr die Taste
volatile bool is_signal_ready = false; 

/**
 * @brief Sendet ein Zeichen ber UART.
 * @param data Das zu sendende Zeichen
 */
void transmit(char data)
{
    while (!(USART3.STATUS & USART_DREIF_bm)) {}
    USART3.TXDATAL = static_cast<uint8_t>(data); // Cast to uint8_t for register
}

/**
 * @brief ISR fr den Timer TCA0 Overflow.
 * Inkrementiert den globalen Millisekundenzhler.
 */
ISR(TCA0_OVF_vect)
{
    millis_counter++;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; ///< Lscht das Interrupt-Flag
}

/**
 * @brief ISR fr die Port C-Tasten.
 * Erkennt gltige Tastendrcke unter Bercksichtigung der Entprellzeit.
 */
ISR(PORTC_PORT_vect)
{
    uint8_t flags = PORTC.INTFLAGS & BUTTON_PINS; ///< Liest die ausgelsten Interrupt-Pins
    PORTC.INTFLAGS = flags; ///< Lscht die Interrupt-Flags

    uint32_t current_time = millis_counter;
    if (current_time - last_press_time < DEBOUNCE_TIME) {
        return; ///< Ignoriert, wenn die Entprellzeit nicht abgelaufen ist
    }
    last_press_time = current_time; ///< Aktualisiert die Zeit des letzten gltigen Tastendrucks

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
int main() // Changed from main(void) to int main()
{
    // Konfiguriert die Pins PC4 bis PC7 als Eingnge mit Pull-Up-Widerstnden und steigender Flanke
    PORTC.DIRCLR = BUTTON_PINS;
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;
    PORTC.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;
    PORTC.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;
    PORTC.PIN7CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;

    cli();

    // Konfiguriert den Timer TCA0 fr eine Basiszeit von 1 ms
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc; 
    TCA0.SINGLE.PER = 1000 - 1; ///< Periode fr 1 ms
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm; 
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; ///< Startet den Timer

    // Konfiguriert USART3 fr die serielle Kommunikation
    USART3.BAUD = static_cast<uint16_t>((F_CPU * 64) / (S * BAUDR)); // Cast to uint16_t
    USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm; ///< Aktiviert Sender und Empfnger
    USART3.CTRLC = USART_CHSIZE_8BIT_gc; 

    PORTB.DIRSET = PIN0_bm; 

    sei(); 

    while (true) { // Use true instead of 1 for C++
        if (is_signal_ready) {
            transmit(button_signal); // Sendet das Signal ber UART
            is_signal_ready = false; 
        }
    }
    return 0; // Added return 0 for int main()
}