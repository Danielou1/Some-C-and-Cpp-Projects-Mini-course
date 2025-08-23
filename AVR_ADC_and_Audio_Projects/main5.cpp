#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h> // Keep for bool type if not using C++ <cstdbool>
#include <stdio.h>   // Keep for sprintf if needed, or replace with C++ streams

#define F_CPU 4000000UL 
#define BAUDR 9600     
#define PINS (PIN0_bm | PIN1_bm | PIN2_bm) 
#define BUFFER 16      

char usart_buffer[BUFFER];
volatile uint8_t buffer_index = 0;
volatile bool data_ready = false;
volatile uint8_t R = 0, G = 0, B = 0;

/**
 * @brief Initialize PWM for RGB LED control.
 * 
 * Configures the TCA0 timer for single-slope PWM mode and sets the output pins.
 */
void init_pwm(); // Removed void from parameter list for C++

/**
 * @brief Set the brightness of the RGB LED.
 * 
 * @param red Brightness value for the Red channel (0-255).
 * @param green Brightness value for the Green channel (0-255).
 * @param blue Brightness value for the Blue channel (0-255).
 */
void set_rgb_led(uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Process the received USART data to extract RGB values.
 * 
 * Parses the received string in the format "R,G,B#" and updates global variables R, G, and B.
 */
void extract_rgb_values(); // Removed void from parameter list for C++

/**
 * @brief Initialize the USART for serial communication.
 * 
 * Configures USART3 with the specified baud rate and enables RX interrupts.
 */
void init_usart(); // Removed void from parameter list for C++

/**
 * @brief USART3 RX complete interrupt service routine.
 * 
 * Reads incoming data into the buffer and sets the `data_ready` flag when the message ends with '#'.
 */
ISR(USART3_RXC_vect);

int main() { // Changed from main(void) to int main()
    init_pwm();
    init_usart();

    sei(); 

    while (true) { // Use true instead of 1 for C++
        if (data_ready) {
            extract_rgb_values();
            set_rgb_led(R, G, B);
            data_ready = false; // Reset the flag for the next message
        }
    }
    return 0; // Added return 0 for int main()
}

void init_pwm() {
    PORTE.DIRSET = PINS;

    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1EN_bm | TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
    TCA0.SINGLE.PER = 255;
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTE_gc;
}

void set_rgb_led(uint8_t red, uint8_t green, uint8_t blue) {
    TCA0.SINGLE.CMP0 = red;
    TCA0.SINGLE.CMP1 = green;
    TCA0.SINGLE.CMP2 = blue;
}

void extract_rgb_values() {
    char *ptr = usart_buffer;
    uint8_t values[3] = {0}; // Temporary array to store converted values
    uint8_t value_index = 0;
    uint8_t current_number = 0;
    
    // Process each character in the buffer
    while (*ptr && value_index < 3) {
        if (*ptr >= '0' && *ptr <= '9') {
            // Convert ASCII to integer and accumulate
            current_number = static_cast<uint8_t>((current_number * 10) + (*ptr - '0')); // Cast to uint8_t
        }
        else if (*ptr == '.' || *ptr == '#') { // Assuming '.' is a delimiter, though not typical for R,G,B#
            // Store the completed number and reset for next value
            if (value_index < 3) {
                values[value_index++] = current_number;
                current_number = 0;
            }
        }
        ptr++;
    }
    
    // Store the last number if buffer ended without delimiter
    if (value_index < 3) {
        values[value_index] = current_number;
    }
    
    // Assign values to RGB variables
    R = values[0];
    G = values[1];
    B = values[2];
}

ISR(USART3_RXC_vect) {
    char received = static_cast<char>(USART3.RXDATAL); // Cast to char

    if (!data_ready) {
        if (received == '#') {
            usart_buffer[buffer_index] = '\0';
            data_ready = true;
            buffer_index = 0;
        } else if (buffer_index < BUFFER - 1) {
            usart_buffer[buffer_index++] = received;
        } else {
            buffer_index = 0; // Reset buffer on overflow
        }
    }
}

void init_usart() {
    USART3.BAUD = static_cast<uint16_t>((F_CPU * 64) / (16UL * BAUDR)); // Cast to uint16_t
    USART3.CTRLB |= USART_RXEN_bm;
    USART3.CTRLC = USART_CHSIZE_8BIT_gc;
    PORTB.DIRCLR = PIN1_bm; // RX as input
    USART3.CTRLA = USART_RXCIE_bm; // Enable RX interrupt
}
