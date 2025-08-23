#include <avr/io.h>
#include <avr/interrupt.h>
#include "song.h"

#define BUTTON_PINS (PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)
#define B_PIN (PIN0_bm)
#define F_CPU (4000000UL)
#define SINE_VALUE (64)
//#define PER_VALUE (64)
#define F_SIGNAL (a1)

volatile uint8_t sine_index = 0;

void init_dac()
{
	VREF.DAC0REF = VREF_REFSEL_2V048_gc;
	//PORTD.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc; // Disable input on PD6
	DAC0.CTRLA = DAC_OUTEN_bm;                  // Enable buffered output to PD6

	// Step 3: Enable DAC
	DAC0.CTRLA |= DAC_ENABLE_bm; // Enable DAC
	DAC0.DATA = sine_table[0];
}

ISR(TCA0_OVF_vect) {
	DAC0.DATA = static_cast<uint16_t>(sine_table[sine_index] << 6);       // Output sine wave value
	sine_index = (sine_index + 1) % 64;       // Increment sine index
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; // Clear interrupt flag
}

/*ISR(PORTA_PORT_vect)
{
	
}*/

int main() { // Changed from main(void) to int main()
	init_dac();
	TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
	TCA0.SINGLE.PER = static_cast<uint16_t>(F_CPU / (F_SIGNAL * SINE_VALUE)); // Cast to uint16_t
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	/*PORTA.DIRCLR = BUTTON_PINS;
	PORTB_DIRCLR = B_PIN;
	
	PORTA.PIN2CTRL &= ~PORT_PULLUPEN_bm; ///< Disable pull-up resistor on button pin
    PORTA.PIN3CTRL &= ~PORT_PULLUPEN_bm;
	PORTA.PIN4CTRL &= ~PORT_PULLUPEN_bm;
	PORTA.PIN5CTRL &= ~PORT_PULLUPEN_bm;
	PORTA.PIN6CTRL &= ~PORT_PULLUPEN_bm;
	PORTA.PIN7CTRL &= ~PORT_PULLUPEN_bm;
	PORTB.PIN0CTRL &= ~PORT_PULLUPEN_bm;
	
	PORTA.PIN2CTRL = PORT_ISC_RISING_gc;
	PORTA.PIN3CTRL = PORT_ISC_RISING_gc; ///< Configure interrupt on rising edge for button
	PORTA.PIN4CTRL = PORT_ISC_RISING_gc;
	PORTA.PIN5CTRL = PORT_ISC_RISING_gc;
	PORTA.PIN6CTRL = PORT_ISC_RISING_gc;
	PORTA.PIN7CTRL = PORT_ISC_RISING_gc;
	PORTB.PIN0CTRL = PORT_ISC_RISING_gc;*/
	
	sei();
    /* Replace with your application code */
    while (true) // Use true instead of 1 for C++
    {
    }
    return 0; // Added return 0 for int main()
}