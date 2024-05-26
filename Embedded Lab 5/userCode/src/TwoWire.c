/**
 * @file    TwoWire.c
 * @brief   Functions for I2C communication.
 *
 * This file contains implementations of functions related to I2C communication.
 * It includes functions to initialize the I2C interface, transfer bytes over I2C,
 * and handle I2C communication errors.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#include <msp430g2553.h>
#include <stdlib.h>
#include <stdint.h>
#include "../inc/TwoWire.h"

// Function prototypes
void initI2C(void);
int16_t transferByte(const uint8_t addr, const uint8_t send);
static int16_t transmit(const uint8_t addr, const uint8_t send);
static int16_t receive(const uint8_t addr);
static int16_t checkAck(void);

/**
 * @brief Initializes the I2C interface.
 * 
 * This function configures the specified pins for I2C communication and initializes
 * the USCI_B0 module for I2C master mode operation.
 */
void initI2C(void)
{
    // Configure P1.6 and P1.7 for I2C
    P1SEL  |= BIT6 + BIT7;
    P1SEL2 |= BIT6 + BIT7;
    
    // Ensure USCI_B0 is in reset before configuring
    UCB0CTL1 = UCSWRST;

    // Set USCI_B0 to master mode I2C mode
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;

    // Configure the baud rate registers for 100kHz when sourcing from SMCLK (SMCLK = 1MHz)
    UCB0BR0 = 10;
    UCB0BR1 = 0;

    // Take USCI_B0 out of reset and source clock from SMCLK
    UCB0CTL1 = UCSSEL_2;
}

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
int16_t transferByte(const uint8_t addr, const uint8_t send)
{
    int16_t ret = 0;

    // Set the slave device address
    UCB0I2CSA = addr;

    // Transmit data if there is any
    ret = transmit(addr, send);

    // Receive data if no error
    if (ret == 0)
    {
        ret = receive(addr);
    }
    else
    {
        // No bytes to receive, send the stop condition
        UCB0CTL1 |= UCTXSTP;
    }

    return ret;
}

/**
 * @brief Transmits a byte over I2C.
 * 
 * @param addr The 7-bit address of the slave device.
 * @param send The byte to be transmitted.
 * 
 * @return Returns 0 on success, or a negative error code.
 * 
 * This function transmits a byte over I2C to the specified slave device address.
 */
static int16_t transmit(const uint8_t addr, const uint8_t send)
{
    int16_t err = 0;

    // Send the start condition
    UCB0CTL1 |= UCTR | UCTXSTT;

    // Wait for the start condition to be sent and ready to transmit interrupt
    while ((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0));

    // Check for ACK
    err = checkAck();

    // If no error and bytes left to send, transmit the data
    if (err == 0) 
    {
        UCB0TXBUF = send;
        while ((IFG2 & UCB0TXIFG) == 0) 
        {
            err = checkAck();
            if (err < 0) {
                break;
            }
        }
    }

    return err;
}

/**
 * @brief Receives a byte over I2C.
 * 
 * @param addr The 7-bit address of the slave device.
 * 
 * @return Returns the received byte or an error code.
 * 
 * This function receives a byte over I2C from the specified slave device address.
 */
static int16_t receive(const uint8_t addr)
{
    int16_t rec = 0;
    uint8_t receive = 0;

    // Send the start condition and switch to receive mode
    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;

    // Wait for the start condition to be sent
    while (UCB0CTL1 & UCTXSTT);

    // If there is only one byte to receive, set the stop bit
    UCB0CTL1 |= UCTXSTP;

    // Check for ACK
    rec = checkAck();

    // If no error and bytes left to receive, receive the data
    if (rec == 0)
    {
        // Wait for the data
        while ((IFG2 & UCB0RXIFG) == 0);

        receive = UCB0RXBUF;
        rec = receive;
    }

    return rec;
}

/**
 * @brief Checks for acknowledgment (ACK) from the slave device.
 * 
 * @return Returns 0 on success, or a negative error code.
 * 
 * This function checks for acknowledgment (ACK) from the slave device after
 * transmitting or receiving data over I2C.
 */
static int16_t checkAck(void)
{
    int16_t err = 0;

    // Check for ACK
    if (UCB0STAT & UCNACKIFG)
    {
        // Stop the I2C transmission
        UCB0CTL1 |= UCTXSTP;

        // Clear the interrupt flag
        UCB0STAT &= ~UCNACKIFG;

        // Set the error code
        err = -1;
    }

    return err;
}