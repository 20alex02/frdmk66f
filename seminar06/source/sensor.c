/*
 * Copyright 2019 NXP
 *
 * sensor.c
 *
 *  Created on: Oct 22, 2019
 *      Author: David Danaj
 */

#include "sensor.h"

#define BUFFER_SIZE 32U         /*! Transfer dataSize */
#define CONTROL_BYTE_SIZE 1U
#define READ_DATA_OFFSET 1U

//
///*******************************************************************************
// * Variables
// ******************************************************************************/
uint8_t masterRxBuffer[BUFFER_SIZE] = {0U};
uint8_t masterTxBuffer[BUFFER_SIZE] = {0U};

volatile uint32_t g_systickCounter;

void setupSysTick() {
	/* Set systick reload value to generate 1ms interrupt */
	if (SysTick_Config(SystemCoreClock / 1000U))
	{
		while (1)
		{
		}
	}
}

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}

/*!
 *  @brief Function that creates a mandatory delay required in some of the APIs such as "bmg250_soft_reset",
 *      "bmg250_set_foc", "bmg250_perform_self_test"  and so on.
 *
 *  @param[in] period_ms  : the required wait time in milliseconds.
 *  @return void.
 *
 */
void delay_ms(uint32_t period_ms)
{
	SysTick_DelayTicks(period_ms);
}

/*!
 *  @brief Function for writing the sensor's registers through SPI bus.
 *
 *  @param[in] cs           : Chip select to enable the sensor.
 *  @param[in] reg_addr     : Register address.
 *  @param[in] reg_data : Pointer to the data buffer whose data has to be written.
 *  @param[in] length       : No of bytes to write.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
	if (length >= BUFFER_SIZE) {
		return 1;
	}
	// TODO: fill masterTxBuffer buffer with data to send according to datasheet (1. byte: Control byte = reg_addr, other bytes: data to send = reg_data
	masterTxBuffer[0] = reg_addr;
	memcpy(masterTxBuffer + 1, reg_data, length);

	// TODO: setup masterXfer structure correctly to send data to sensor
	dspi_transfer_t masterXfer;
	/* Start master transfer, send data to slave */
	masterXfer.txData      = masterTxBuffer;
	masterXfer.rxData      = masterRxBuffer;
	masterXfer.dataSize    = length + 1;
	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcsContinuous; // TODO more flags can be added by ORing them together (using | operator)

	// TODO: use DSPI_MasterTransferBlocking function to send data to sensor
	// TODO: check function return value as well and return code according to @retval in the documentation of this method
	// DSPI_MasterTransferBlocking returns kStatus_Success when transfer was successful
	return DSPI_MasterTransferBlocking(SPI0, &masterXfer) == kStatus_Success ? 0 : 1;
}

/*!
 *  @brief Function for reading the sensor's registers through SPI bus.
 *
 *  @param[in] cs       : Chip select to enable the sensor.
 *  @param[in] reg_addr : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   : No of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
	if (length >= BUFFER_SIZE) {
		return 1;
	}
	// TODO: setup masterXfer structure correctly to send data to sensor and received data from sensor - check BMP 280 datasheet for more info
	masterTxBuffer[0] = reg_addr;

	dspi_transfer_t masterXfer;
	masterXfer.txData      = masterTxBuffer;
	masterXfer.rxData      = masterRxBuffer;
	masterXfer.dataSize    = length + 1;
	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcsContinuous; // TODO more flags can be added by ORing them together (using | operator)

	// TODO: use DSPI_MasterTransferBlocking function to send data to sensor
	// TODO: check function return value as well and return code according to @retval in the documentation of this method
	// DSPI_MasterTransferBlocking returns kStatus_Success when transfer was successful
	if (DSPI_MasterTransferBlocking(SPI0, &masterXfer) == kStatus_Success) {
		memcpy(reg_data, masterRxBuffer + 1, length);
		return 0;
	}
	// TODO: set received data into "out" argument reg_data
	// HINT: When sending data to sensor (Control byte), data from sensor are read simultaneously - be aware of this
	return 1;
}
