/*
 ***************************************************************************************************************************
 * @author: David Lotz
 * @file:   I2C_LCD.h
 * @date:   28.09.2022
 *
 * This module uses the I2C-Bus to control a HD44780 1602 LCD via the HW-061 I2C-Serial Interface with PCF8574 I/O Expander.
 * 
 * HD44780 Datasheet:
 * https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 * PCF8574 I/O Expander Datasheet:
 * https://www.ti.com/lit/ds/symlink/pcf8574.pdf
 *
 * Wiring Diagram:
 * http://www.handsontec.com/dataspecs/module/I2C_1602_LCD.pdf
 *
 * *************************************************************************************************************************
 *
 * Lecturer:
 * Jakob Czekansky (jakob.czekansky@mni.thm.de)
 *
 * Mikroprozessortechnik
 * Technische Hochschule Mittelhessen
 *
 ***************************************************************************************************************************
 */

// INCLUDES //
#define F_CPU 4000000	// Peripheral Clock Speed for correct delay functionality
#include <util/delay.h>
#include <I2C_LCD.h>

// DEFINES //
#define DISPLAY_ADDRESS 0x27	// Only applies if Pins A1, A2 and A3 of the HW-061 are open (connected to Vdd)

#define RS	0b00000001		// RS Enable
#define RW	0b00000010		// RW Enable
#define E	0b00000100		// E  Enable
#define BT	0b00001000		// BT Enable
#define D0	0b00000001		// D0 Enable
#define D1	0b00000010		// D1 Enable
#define D2	0b00000100		// D2 Enable
#define D3	0b00001000		// D3 Enable
#define D4	0b00010000		// D4 Enable
#define D5	0b00100000		// D5 Enable
#define D6	0b01000000		// D6 Enable
#define D7	0b10000000		// D7 Enable

// VARIABLES //
volatile i2c_status status = SUCCESS;
volatile uint8_t display_state = 0x00;

// PRIVATE FUNCTION DECLARATIONS //
static i2c_status lcd_write_data(uint8_t data, bool rs, bool rw, bool init);

// PUBLIC FUNCTIONS //

/*
	Initializes the LCD-Display by sending the required Initialization Sequence and 
	following commands:
	- Run in 4-Bit Mode
	- 2 Lines, 5x8 Font Size
	- Enable the display (Show written characters)
	- Clear the display (Remove all written characters)
	- Set the cursor to move from left to right (after each write)
	- Enables the backlight
	
	@param NONE
	@return i2c_status SUCCESS if operation succeeded. Any other: See AVR128DB48_I2C Module.
*/
i2c_status lcd_init() { // Removed void from parameter list for C++
	
	i2c_init();				// Init I2C-Bus
		
	status = i2c_write_byte(DISPLAY_ADDRESS, 0x00);	// Clear I2C I/O-Expander
	if(status != SUCCESS)
		return status;
	
	_delay_ms(50);			// Waiting phase after power-on of LCD
	
	// 4-Bit Initialization sequence (Figure 24 of the HD44780 Datasheet) //
	status = lcd_write_data(static_cast<uint8_t>(D4 + D5), true, false, true); // Cast to uint8_t
	if (status != SUCCESS)
		return status;
	_delay_us(5000);
	
	status = lcd_write_data(static_cast<uint8_t>(D4 + D5), true, false, true); // Cast to uint8_t
	if (status != SUCCESS)
		return status;
	_delay_us(110);
	
	status = lcd_write_data(static_cast<uint8_t>(D4 + D5), true, false, true); // Cast to uint8_t
	if (status != SUCCESS)
		return status;
	_delay_us(50);

	// Function Set Instruction //
	status = lcd_write_data(D5, false, false, true);		// Put LCD to 4-Bit Mode
	if (status != SUCCESS)		
		return status;
	_delay_us(37);

	status = lcd_write_data(static_cast<uint8_t>(D3 + D5), false, false, false);	// 2 Lines, 5x8 Font size
	if (status != SUCCESS)		
		return status;
	_delay_us(37);
	
	status = lcd_enable(true);		// Enable Display
	if (status != SUCCESS)
		return status;
	
	status = lcd_clear();			// Clear Display
	if(status != SUCCESS)
		return status;
	
	status = lcd_leftToRight();		// Cursor moves from left to right
	if(status != SUCCESS)
		return status;
		
	status = lcd_backlight(true);	// Enable backlight
	if(status != SUCCESS)
		return status;
		
	return SUCCESS;
}

/*
	Enables / Disables the display.
	@param enable true: show written characters; false: hide written characters.
	@return i2c_status SUCCESS if operation succeeded. Any other: See AVR128DB48_I2C Module.
*/
i2c_status lcd_enable(bool enable) {
	if (enable) {
		status = lcd_write_data(static_cast<uint8_t>(D2 + D3), false, false, false); // Cast to uint8_t
		if (status != SUCCESS)	// Enable Display
			return status;
	}
	else {
		status = lcd_write_data(D3, false, false, false);			// Disable Display
		if (status != SUCCESS)			// Disable Display
			return status;
	}
	_delay_us(37);
	
	return SUCCESS;
}

/*
	Enables / Disables the backlight.
	
	@param enable true: enable backlight; false: disable backlight.
	@return i2c_status SUCCESS if operation succeeded. Any other: See AVR128DB48_I2C Module.
*/
i2c_status lcd_backlight(bool enable) {
	if (enable) {
		display_state |= BT;
		status = lcd_write_data(static_cast<uint8_t>(D2 + D3), false, false, false); // Cast to uint8_t
		if (status != SUCCESS)			// Enable Display
			return status;
	}
	else {
		display_state &= static_cast<uint8_t>(~BT); // Cast to uint8_t
		status = lcd_write_data(D3, false, false, false);			// Disable Display
		if (status != SUCCESS)			// Disable Display
			return status;
	}
	_delay_us(37);
		
	return SUCCESS;
}

/*
	Clears any written characters written to the display up until the call of this function.
	
	@param NONE
	@return i2c_status SUCCESS if operation succeeded. Any other: See AVR128DB48_I2C Module.
*/
i2c_status lcd_clear() { // Removed void from parameter list for C++
	if (lcd_write_data(D0, false, false, false) != SUCCESS)				// Clear Display
		return ERROR;
	_delay_us(1600);
	
	return SUCCESS;
}


static const uint8_t row_offset[] = {0x00, 0x40};	// Offset of each line in character memory

/*
	Moves the cursor to the specified position on the display.
	Any next write will occur at this position and possibly overwrite
	characters that have been already written to this position.
	
	@param x A value from 0 to 15. Specifies the horizontal position (column).
	@param y A value from 0 to 1. Specifies the vertical position (row).
	@return i2c_status SUCCESS if operation succeeded. Any other: See AVR128DB48_I2C Module.
*/
i2c_status lcd_moveCursor(uint8_t x, uint8_t y) {
	
	// Constrain Columns //
	if (x > 15)
		x = 15;
	// Constrain Rows //
	if (y > 1)
		y = 1;
		
	if (lcd_write_data(static_cast<uint8_t>(D7 + row_offset[y] + x), false, false, false) != SUCCESS)	// Move Cursor (DDRAM Address)
		return ERROR;
	_delay_us(37);
	
	return SUCCESS;
}

/*
	Writes a specified character to the current cursor-position.
	The cursor will be incremented or decremented (only the horizontal position) after one such write;
	dependent on whether lcd_leftToRight() (=incrementing) or lcd_rightToLeft() (=decrementing) was last executed.
	
	@param character The ASCII-value of the character to be written.
	@return i2c_status SUCCESS if operation succeeded. Any other: See AVR128DB48_I2C Module.
*/
i2c_status lcd_putChar(char character) {
	if (lcd_write_data(static_cast<uint8_t>(character), true, false, false) != SUCCESS) // Cast to uint8_t
		return ERROR;
	_delay_us(41);
		
	return SUCCESS;
}

/*
	Writes a specified string to the current cursor-position by writing each character one after another.
	The cursor will be incremented or decremented (only the horizontal position) after each such write;
	dependent on whether lcd_leftToRight() (=incrementing) or lcd_rightToLeft() (=decrementing) was last executed.
	
	@param character The ASCII-value of the character to be written.
	@return i2c_status SUCCESS if operation succeeded. Any other: See AVR128DB48_I2C Module.
*/
i2c_status lcd_putString(char* string) {
	while(*string != 0x0) {		
		if (lcd_write_data(static_cast<uint8_t>(*string), true, false, false) != SUCCESS) // Cast to uint8_t
			return ERROR;
			
		string++;
		_delay_us(41);
	}
	
	return SUCCESS;
}

/*
	Specifies the move direction of the cursor:
	The cursors horizontal position will be incremented after each write.
	
	@param NONE
	@return i2c_status SUCCESS if operation succeeded. Any other: See AVR128DB48_I2C Module.
*/
i2c_status lcd_leftToRight() { // Removed void from parameter list for C++
	if (lcd_write_data(static_cast<uint8_t>(D1 + D2), false, false, false) != SUCCESS)	// Cursor moves from left to right
		return ERROR;
	_delay_us(37);
	
	return SUCCESS;
}

/*
	Specifies the move direction of the cursor:
	The cursors horizontal position will be decremented after each write.
	
	@param NONE
	@return i2c_status SUCCESS if operation succeeded. Any other: See AVR128DB48_I2C Module.
*/
i2c_status lcd_rightToLeft() { // Removed void from parameter list for C++
	if (lcd_write_data(D2, false, false, false) != SUCCESS)			// Cursor moves from right to left
		return ERROR;
	_delay_us(37);
	
	return SUCCESS;
}

// PRIVATE FUNCTIONS //
static i2c_status lcd_write_data(uint8_t data, bool rs, bool rw, bool init) {
	
	// Split Data in Low and High half //
	uint8_t high_data = static_cast<uint8_t>(data & 0xF0); // Cast to uint8_t
	uint8_t low_data = static_cast<uint8_t>((data & 0x0F) << 4); // Cast to uint8_t
	

	// Check if RS or RW shall be set
	uint8_t control = 0;
	if (rs)
		control += RS;
	if (rw)
		control += RW;
		
	// Send Bits 7 - 4 //
	if (i2c_write_byte(DISPLAY_ADDRESS, static_cast<uint8_t>(high_data + control + display_state + E)) != SUCCESS)
		return ERROR;
	_delay_us(30);
	if (i2c_write_byte(DISPLAY_ADDRESS, static_cast<uint8_t>(high_data + control + display_state)) != SUCCESS)		// Pull enable low
		return ERROR;
	_delay_us(37);
		
	// Send Bits 3 - 0 (Only if not in initialization sequence) //
	if (!init) {
		if (i2c_write_byte(DISPLAY_ADDRESS, static_cast<uint8_t>(low_data + control + display_state + E)) != SUCCESS)
			return ERROR;
		_delay_us(30);
		if (i2c_write_byte(DISPLAY_ADDRESS, static_cast<uint8_t>(low_data + control + display_state)) != SUCCESS)		// Pull enable low
			return ERROR;
	}
	return SUCCESS;
}
