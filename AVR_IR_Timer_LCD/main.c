/**********************************************************************************************************
 * @file main.c
 * @brief Implementierung eines Timers mit IR-Fernbedienung basierend auf dem NEC-Protokoll.
 *
 * Dieses Programm steuert einen Timer, der über eine Infrarot-Fernbedienung bedient wird.
 * Die Funktionen umfassen Start/Stop, Inkrementieren, Dekrementieren und Setzen eines spezifischen Werts.
 * Die LED zeigt das Ende des Timers an.
 *
 * @author Danielou Mounsande
 * @date 2025-01-26
 ************************************************************************************************************/
#include <avr/io.h>
#define F_CPU 4000000UL    
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C_LCD.h"


// Definitions
#define NEC_START_MIN 8500        ///< Minimum duration in µs for start pulse
#define NEC_START_MAX 9500        ///< Maximum duration in µs for start pulse
#define NEC_BIT_0_MIN 1000        ///< Minimum duration in µs for bit 0
#define NEC_BIT_0_MAX 1500        ///< Maximum duration in µs for bit 0
#define NEC_BIT_1_MIN 2000        ///< Minimum duration in µs for bit 1
#define NEC_BIT_1_MAX 2600        ///< Maximum duration in µs for bit 1

// Global Variables
volatile uint16_t remaining_time = 0;  ///< Remaining time in seconds
volatile uint8_t timer_running = 0;    ///< Indicates if the timer is running
volatile uint32_t nec_data = 0;        ///< Storage for NEC data
volatile uint8_t bit_index = 0;        ///< Current bit index
volatile uint8_t decoding = 0;         ///< Decoding status
char buf[32];                          ///< Buffer for LCD display

// Function Prototypes
void update_lcd();
void process_command(uint8_t command);
char* integer_to_string(char *buf, int32_t num, int base);
void configure_timer();
void configure_ir_receiver();
void handle_lcd_update();
void handle_timer_overflow();
void handle_ir_signal();

/**
 * @brief Converts an integer to a string representation.
 *
 * @param buf Buffer to store the resulting string.
 * @param num The integer to convert.
 * @param base The numerical base for the conversion.
 * @return char* Pointer to the resulting string.
 */
char* integer_to_string(char *buf, int32_t num, int base) {
    bool isNegative = false;
    int digitCounter = 0;

    if (num == 0) {
        buf[digitCounter++] = '0';
    }

    if (num < 0) {
        isNegative = true;
        num = -num;
    }

    while (num > 0) {
        buf[digitCounter++] = '0' + num % base;
        num /= base;
    }

    if (isNegative) {
        buf[digitCounter++] = '-';
    }

    buf[digitCounter] = '\0';

    // Reverse the string
    for (int i = 0, j = digitCounter - 1; i < j; i++, j--) {
        char temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
    }

    return buf;
}

/**
 * @brief Updates the content of the LCD.
 */
void update_lcd() {
    lcd_clear();
    lcd_putString(integer_to_string(buf, remaining_time, 10));
    lcd_moveCursor(0, 0);
}

/**
 * @brief Processes commands received via NEC IR protocol.
 *
 * @param command The command byte extracted from the NEC data.
 */
void process_command(uint8_t command) {
    switch (command) {
        case 0x40: // Start/Stop
            timer_running ^= 1; // Toggle start/stop
            break;
        case 0x46: // Increment
            remaining_time++;
            update_lcd();
            break;
        case 0x15: // Decrement
            if (remaining_time > 0) remaining_time--;
            update_lcd();
            break;
        default: // Commands 0-9
            if (command >= 0x16 && command <= 0x1C) {
                remaining_time = command - 0x16;
                update_lcd();
            }
            break;
    }
}

/**
 * @brief ISR for detecting edges on the IR receiver pin.
 */
ISR(PORTC_PORT_vect) {
    static uint16_t pulse_width = 0;
    uint16_t current_time = TCA0.SINGLE.CNT;
    TCA0.SINGLE.CNT = 0; // Reset the timer counter

    if (PORTC.IN & PIN3_bm) { // Rising edge
        pulse_width = current_time;
    } else { // Falling edge
        if (!decoding) {
            if (pulse_width > NEC_START_MIN && pulse_width < NEC_START_MAX) { // Start pulse detected
                decoding = 1;
                bit_index = 0;
                nec_data = 0;
            }
        } else {
            if (pulse_width > NEC_BIT_0_MIN && pulse_width < NEC_BIT_0_MAX) { // Bit 0
                nec_data <<= 1;
            } else if (pulse_width > NEC_BIT_1_MIN && pulse_width < NEC_BIT_1_MAX) { // Bit 1
                nec_data = (nec_data << 1) | 1;
            }

            bit_index++;
            if (bit_index == 32) { // All bits received
                uint8_t command = (nec_data >> 16) & 0xFF;
                uint8_t inverted_command = (nec_data >> 8) & 0xFF;

                if ((command ^ inverted_command) == 0xFF) { // Command verification
                    process_command(command);
                }
                decoding = 0; // End of decoding
            }
        }
    }

    PORTC.INTFLAGS = PIN3_bm; // Clear interrupt flag
}

/**
 * @brief ISR for timer overflow.
 */
ISR(TCA0_OVF_vect) {
    if (timer_running && remaining_time > 0) {
        remaining_time--;
        update_lcd();

        if (remaining_time == 0) {
            PORTE.OUTSET = PIN0_bm; // Turn on the LED
            timer_running = 0;
        }
    }
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; // Clear interrupt flag
}

/**
 * @brief Configures the timer for countdown functionality.
 */
void configure_timer() {
    TCA0.SINGLE.PER = 100000; //overflow us
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV4_gc;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
}

/**
 * @brief Configures the IR receiver pin.
 */
void configure_ir_receiver() {
    PORTC.DIRCLR = PIN3_bm; // Set PC3 as input
    PORTC.PIN3CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
}

/**
 * @brief Main function.
 *
 * Initializes peripherals and enters an infinite loop to process IR signals and timer events.
 */
int main(void) {
    lcd_init();
    PORTE.DIRSET = PIN0_bm; // Set LED as output
    PORTE.OUTCLR = PIN0_bm;

    configure_timer();
    configure_ir_receiver();

    sei(); // Enable global interrupts
    update_lcd();

    while (1) {
        // Main loop
    }
}
