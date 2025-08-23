/*
 ***********************************************************************************
 * @author: David Lotz
 * @file:   AVR128DB48_I2C.h
 * @date:   20.09.2022
 *
 * This module initializes the I2C Bus as Master and provides functions for usage.
 *
 * FYI: This I2C Implementation is currently limited to Normal Mode (100kHz Speed),
 *      due to the CPU-Speeds required for faster rates.
 *
 * *********************************************************************************
 *
 * Lecturer:
 * Jakob Czekansky (jakob.czekansky@mni.thm.de)
 *
 * Mikroprozessortechnik
 * Technische Hochschule Mittelhessen
 *
 ***********************************************************************************
  
  Connections:
  SDA - PA2
  SCL - PA3
  
  1. Call i2c_init() before using any other function.                                                  
  2. Use i2c_read() or i2c_write() for transmitting and receiving data.
*/


#ifndef ARV128DB48_I2C_H_
#define ARV128DB48_I2C_H_

// INCLUDES //
#include <avr/io.h>


// ENUMS //
typedef enum {
	SUCCESS,			// Transmission was successful
	ERROR,				// An error occurred during transmission
	ERROR_NOT_READY,	// An error occurred due to the bus or master being unaivailable
	NACK,				// Received a NACK, indicating the slave was not able to decipher the send data or the wrong address was used
	ARBITRATION_LOST	// Arbitration was lost during transmission
} i2c_status;

/*
typedef enum {
	NORMAL_MODE,	// Bus operating at 100kHz
	FAST_MODE,		// Bus operating at 400kHz
	FAST_MODE_PLUS	// Bus operating at 1MHz
} i2c_mode;
*/

// FUNCTION DECLARATIONS //
//void i2c_init(i2c_mode mode);
void i2c_init(); // Removed void from parameter list for C++

i2c_status i2c_write(uint8_t address, uint8_t* data, uint8_t length);

i2c_status i2c_write_byte(uint8_t address, uint8_t data);

i2c_status i2c_read(uint8_t address, uint8_t* data, uint8_t length);

i2c_status i2c_read_byte(uint8_t address, uint8_t* data);


#endif /* ARV128DB48_I2C_H_ */