/*
 ***********************************************************************************
 * @author: David Lotz
 * @file:   AVR128DB48_I2C.c
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
 */

// INLCUDES //
#include "AVR128DB48_I2C.h"
#define F_CPU 4000000
#include <util/delay.h>

// DEFINES //
#define I2C_WRITE		0		// Write Bit in Address
#define I2C_READ		1		// Write Bit in Address

// Variables //
volatile i2c_status status;

// PRIVATE FUNCTION DECLARATIONS //
static void			wait_for_state_change(); // Removed void from parameter list for C++
static i2c_status	check_errors(); // Removed void from parameter list for C++

// PUBLIC FUNCTIONS //
/*
*	Initializes the I2C Bus in Normal Mode and this device as Master
*	@return None
*/
void i2c_init() { // Removed void from parameter list for C++
	
	// I2C Configuration //
	TWI0.CTRLA = TWI_SDAHOLD_50NS_gc;	// Set Holdtime to 50ns
	
	// Enable Run in Debug //
	TWI0.DBGCTRL = TWI_DBGRUN_bm;
																					
	// Clear Master Status Register //
	TWI0.MSTATUS = TWI_RIF_bm |				// Clear Read Interrupt Flag
				   TWI_WIF_bm |				// Clear Write Interrupt Flag
				   TWI_CLKHOLD_bm |			// Clear Clockhold Flag
				   TWI_RXACK_bm |			// Clear Acknowledge Flag
				   TWI_ARBLOST_bm |			// Clear Arbitration Lost Flag
				   TWI_BUSERR_bm |			// Clear Bus Error Flag
				   TWI_BUSSTATE_IDLE_gc;	// Force Master into IDLE-Mode
	
	// Master Configuration //
	TWI0.MBAUD = 15;		// Calculated Baud-Setting based on F_CPU = 4000000 and T_r (Rise time for both SDA and SCL, found in AVR128DB48 Data sheet -> Electrical Characteristics)
							// Formula is found in AVR128DB48 Data sheet -> Two-Wire Interface
							// Results in ~100kHz
	
	TWI0.MCTRLA = TWI_ENABLE_bm;	// Use this device as Master withput any Interrupts
}

/*
*	Writes data to the specified device address.
*
*	@param address Address of the target device
*	@param data Data as Byte-Array to be send
*	@param length Length of the Data Byte-Array
*	@return i2c_status Status code after execution
*/
i2c_status i2c_write(uint8_t address, uint8_t* data, uint8_t length) {
	
	// Check if Master is not in idle //
	if((TWI0.MSTATUS & TWI_BUSSTATE_IDLE_gc) != TWI_BUSSTATE_IDLE_gc)
		return ERROR_NOT_READY;
	
	// Transmit Address //
	TWI0.MADDR = static_cast<uint8_t>((address << 1) | I2C_WRITE);	// Start write operation by writing the address to the MADDR register, 
												// initiating the transmission
	
	// Wait for change in bus state //
	wait_for_state_change();
	
	// Check any bus errors //
	check_errors();

	// Transmit Data //
	uint8_t pos = 0;
	while(pos < length) {
		
		TWI0.MDATA = data[pos++];
		
		wait_for_state_change();
		
		// Check for NACK //
		if(static_cast<bool>(TWI0.MSTATUS & TWI_RXACK_bm)) { // Cast to bool
			TWI0.MCTRLB = TWI_MCMD_STOP_gc;		// -> Stop transmission
			return NACK;
		}
	}
	
	// Stop Transmission //
	TWI0.MCTRLB = TWI_MCMD_STOP_gc;
	
	return SUCCESS;
}

/*
*	Writes one byte of data to the specified device address.
*	A transmission takes approximately 300 microseconds.
*
*	@param address Address of the target device
*	@param data Data-Byte
*	@return i2c_status Status code after execution
*/
i2c_status i2c_write_byte(uint8_t address, uint8_t data) {

	// Check if Master is not in idle //
	if((TWI0.MSTATUS & TWI_BUSSTATE_IDLE_gc) != TWI_BUSSTATE_IDLE_gc)
		return ERROR_NOT_READY;
	
	// Transmit Address //
	TWI0.MADDR = static_cast<uint8_t>((address << 1) | I2C_WRITE);	// Start write operation by writing the address to the MADDR register,
												// initiating the transmission
	
	// Wait for change in bus state //
	wait_for_state_change();
	
	// Check any bus errors //
	status = check_errors();
	if(status != SUCCESS)
		return status;
	
	// Transmit Data //
	TWI0.MDATA = data;
		
	// Wait for change in bus state //
	wait_for_state_change();
	
	// Check any bus errors //
	status = check_errors();
	if(status != SUCCESS)
		return status;
	
	// Stop Transmission //
	TWI0.MCTRLB = TWI_MCMD_STOP_gc;
	
	return SUCCESS;	
}

/*
*	Read data from the specified device address.
*
*	@param address Address of the target device
*	@param data Byte-Array to save read data
*	@param length Length of the Data Byte-Array (length of the expected answer)
*	@return i2c_status Status code after execution
*/
i2c_status i2c_read(uint8_t address, uint8_t* data, uint8_t length) {

	// Check if Master is not in idle //
	if((TWI0.MSTATUS & TWI_BUSSTATE_IDLE_gc) != TWI_BUSSTATE_IDLE_gc)
		return ERROR_NOT_READY;
	
	// Transmit Address //
	TWI0.MADDR = static_cast<uint8_t>((address << 1) + I2C_READ);		// Start read operation by writing the address to the MADDR register,
												// initiating the transmission
	
	// Wait for change in bus state //
	wait_for_state_change();
	
	// Check any bus errors //
	status = check_errors();
	if(status != SUCCESS)
		return status;
	
	// Release Clock //
	TWI0.MSTATUS = TWI_CLKHOLD_bm;
		
	uint8_t pos = 0;
	while (pos < length)
	{
		// Wait until data is received //
		wait_for_state_change();
		
		// Store incoming byte //
		data[pos++] = TWI0.MDATA;
		
		// Send ACK and get ready to read next byte, if transmission is still ongoing //
		if (pos != length) {
			TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
		}
	}
	
	// Finish transmission with NACK and stop it //
	TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
		
	return SUCCESS;		
}

/*
*	Reads one byte of data from the specified device address.
*
*	@param address Address of the target device
*	@param data Data-Byte storage location
*	@return i2c_status Status code after execution
*/
i2c_status i2c_read_byte(uint8_t address, uint8_t* data) {
	
	// Check if Master is not in idle //
	if((TWI0.MSTATUS & TWI_BUSSTATE_IDLE_gc) != TWI_BUSSTATE_IDLE_gc)
		return ERROR_NOT_READY;
	
	// Transmit Address //
	TWI0.MADDR = static_cast<uint8_t>((address << 1) | I2C_READ);	// Start write operation by writing the address to the MADDR register,
	// initiating the transmission
	
	// Wait for change in bus state //
	wait_for_state_change();
	
	// Check any bus errors //
	status = check_errors();
	if(status != SUCCESS)
		return status;
	
	// Release Clock //
	TWI0.MSTATUS = TWI_CLKHOLD_bm;
	
	// Wait until data is received //
	wait_for_state_change();
	
	// Read Data //
	data[0] = TWI0.MDATA;
	
    //NACK and STOP the bus
    TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
	
	return SUCCESS;
}

// PRIVATE FUNCTIONS //
static void wait_for_state_change() { // Removed void from parameter list for C++
	// Wait for completion of address transmission / for an error //	// Wait until one state is true:
	while (!(static_cast<bool>(TWI0.MSTATUS & TWI_CLKHOLD_bm) ||							// Clockhold is active	(Transmission was successful)
	static_cast<bool>(TWI0.MSTATUS & TWI_BUSERR_bm)   ||									// A bus error occured
	static_cast<bool>(TWI0.MSTATUS & TWI_ARBLOST_bm)  ||									// Arbitration is lost
	static_cast<bool>((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) == TWI_BUSSTATE_BUSY_gc))	// Bus switches to busy
	);
}

static i2c_status check_errors() { // Removed void from parameter list for C++
	// Check for errors //
	if(static_cast<bool>(TWI0.MSTATUS & TWI_RXACK_bp)) {											// Check for NACK
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;											// -> Stop transmission
		return NACK;
	}
	if(static_cast<bool>(TWI0.MSTATUS & TWI_ARBLOST_bm)) {											// Check for arbitration lost
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;											// -> Stop transmission
		return ARBITRATION_LOST;
	}
	if(static_cast<bool>(TWI0.MSTATUS & TWI_BUSERR_bm)) {											// Check for bus error
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;											// -> Stop transmission
		return ERROR;
	}
	if(static_cast<bool>((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) == TWI_BUSSTATE_BUSY_gc)) {			// Check if bus is busy
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;											// -> Stop transmission
		return ERROR_NOT_READY;
	}
	
	return SUCCESS;
}
