#include <avr/io.h>
#include <avr/interrupt.h>
// #include <stdio.h> // Not strictly needed for this C++ conversion unless printf is used
#include "song.h"

#define F_CPU 4000000UL

#define TCA_PER(x) ((uint16_t)(F_CPU / ((x) * 64) - 1))
#define DEBOUNCE_TIME 50

#define BUTTON_PINS (PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)

volatile uint8_t debounce_flag = 0;
volatile uint16_t millis = 0;
volatile uint8_t current_note = mute;

void init_dac() {
    VREF.DAC0REF = VREF_REFSEL_2V048_gc;
    DAC0.CTRLA = DAC_OUTEN_bm | DAC_ENABLE_bm;
    DAC0.DATA = sine_table[0] << 6;  // Start with no sound (mute)
}

void init_debouncer() {
    TCA1.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc | TCA_SINGLE_ENABLE_bm;
    TCA1.SINGLE.PER = 1000 - 1;
    TCA1.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
}

ISR(TCA1_OVF_vect) {
    millis++;
    debounce_flag = 0;
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

//ISR for buttons at Port B
ISR(PORTB_PORT_vect) {
    if (!debounce_flag) {
        debounce_flag = 1;
        if (PORTB.IN & PIN0_bm) {   
                TCA0.SINGLE.PER = TCA_PER(a);
                TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
            } else {
            current_note = mute;
            TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
        }
        PORTB.INTFLAGS = PIN0_bm;
    }
}

//ISR for buttons at Port A
ISR(PORTA_PORT_vect) {
    if (!debounce_flag) {
        debounce_flag = 1;
        if (PORTA.IN & PIN2_bm) {
                TCA0.SINGLE.PER = TCA_PER(c);
                TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
            } else if (PORTA.IN & PIN3_bm) {
                TCA0.SINGLE.PER = TCA_PER(d);
                TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
            } else if (PORTA.IN & PIN4_bm) {
                TCA0.SINGLE.PER = TCA_PER(e);
                TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
            } else if (PORTA.IN & PIN5_bm) {
                TCA0.SINGLE.PER = TCA_PER(f);
                TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
            } else if (PORTA.IN & PIN6_bm) {
                TCA0.SINGLE.PER = TCA_PER(g);
                TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
            } else if (PORTA.IN & PIN7_bm) {
                TCA0.SINGLE.PER = TCA_PER(h);
                TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
            }
            } else {
            current_note = mute;
            TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
        }
        PORTA.INTFLAGS = BUTTON_PINS;
    }
}

//ISR for timer after an Overflow of the counter has occurred
ISR(TCA0_OVF_vect) {
    static uint8_t current = 0;
    if (current >= 64) current = 0;
    DAC0.DATA = sine_table[current++] << 6;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

int main() { // Changed from main(void) to main()
    // Configuration of buttons as input (piano keys)
    PORTA.DIRCLR = BUTTON_PINS;
    PORTB.DIRCLR = PIN0_bm;
    PORTA.PIN2CTRL = PORT_ISC_RISING_gc;
    PORTA.PIN3CTRL = PORT_ISC_RISING_gc;
    PORTA.PIN4CTRL = PORT_ISC_RISING_gc;
    PORTA.PIN5CTRL = PORT_ISC_RISING_gc;
    PORTA.PIN6CTRL = PORT_ISC_RISING_gc;
    PORTA.PIN7CTRL = PORT_ISC_RISING_gc;
    PORTB.PIN0CTRL = PORT_ISC_RISING_gc;

    init_dac();
    init_debouncer();

    // Timer/Counter TCA0 Configuration
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;
    TCA0.SINGLE.PER = TCA_PER(mute);

    sei();

    while (1) {
        // Main loop remains empty; interrupts handle button presses
    }
    return 0; // Added return 0 for int main()
}