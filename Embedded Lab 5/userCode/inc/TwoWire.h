/**
 * @file    TwoWire.h
 * @brief   Header file for I2C communication.
 *
 * This file contains function declarations for I2C communication.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#ifndef TWOWIRE_H
#define TWOWIRE_H

#include <stdint.h>

/**
 * @brief Initializes the I2C interface.
 * 
 * This function initializes the I2C interface for communication.
 */
void initI2C(void);

/**
 * @brief Transfers a byte over I2C.
 * 
 * @param addr The 7-bit address of the slave device.
 * @param send The byte to be transmitted.
 * 
 * @return Returns the received byte or an error code.
 * 
 * This function transfers a byte over I2C to the specified slave device address.
 */
int16_t transferByte(const uint8_t addr, const uint8_t send);

#endif /* TWOWIRE_H */
