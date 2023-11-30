/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    SPI2_Template.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

#define TRANSFER_SIZE 64U
#define CMD_WRITE ((uint32_t)0xccU)
#define CMD_READ ((uint32_t)0x55U)
#define WRITE_SIZE 16

typedef enum {
	HEADER_RECEIVED,
	READ,
	READ_COMPLETED,
	WRITE_COMPLETED,
	STATE_COUNT
} State;

dspi_slave_handle_t g_s_handle;
uint8_t slaveRxData[TRANSFER_SIZE] = {0U};
uint8_t slaveTxData[TRANSFER_SIZE] = {0U};
volatile bool uart_flag = false;
uint8_t uart_data[WRITE_SIZE] = {0};

uint8_t ADC_prepared[2];
volatile uint16_t adc_value = 0;
volatile bool triggered_adc = false;

/* ADC1_IRQn interrupt handler */
void ADC1_IRQHANDLER(void) {
	if (ADC16_GetChannelStatusFlags(ADC1_PERIPHERAL, 0U) & kADC16_ChannelConversionDoneFlag) {
		triggered_adc = true;
		adc_value = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, 0U);
		ADC_prepared[0] = adc_value >> 8;
		ADC_prepared[1] = adc_value;
	}

	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
	 Store immediate overlapping exception return operation might vector to incorrect interrupt. */
	#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
	#endif
}


void DSPI_SlaveUserCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
	static State state = HEADER_RECEIVED;
	static uint8_t counter = 0;
	dspi_transfer_t *slaveXfer = (dspi_transfer_t *) userData;
	PRINTF("State: %d\r\n", state);
    if (status == kStatus_Success) {
    	slaveXfer->dataSize = 1;
    	switch (state) {
		case HEADER_RECEIVED:
			PRINTF("header received\r\n");
			switch (slaveXfer->rxData[0]) {
			case CMD_READ:
				PRINTF("cmd read setup\r\n");
				slaveXfer->txData[0] = ADC_prepared[0];
				state = READ;
				break;
			case CMD_WRITE:
				PRINTF("cmd write setup\r\n");
				slaveXfer->dataSize = WRITE_SIZE;
				state = WRITE_COMPLETED;
				break;
			default:
				PRINTF("no cmd setting up counter\r\n");
				slaveXfer->txData[0] = counter;
			}
			break;
		case READ:
			PRINTF("read 1 completed\r\n");
			slaveXfer->txData[1] = ADC_prepared[1];
			state = READ_COMPLETED;
			break;
		case WRITE_COMPLETED:
			PRINTF("write completed\r\n");
			uart_flag = true;
			memcpy(uart_data, slaveXfer->rxData, WRITE_SIZE);
			++counter;
			slaveXfer->txData[0] = counter;
			state = HEADER_RECEIVED;
			break;
		case READ_COMPLETED:
			PRINTF("read 2 completed\r\n");
			++counter;
			slaveXfer->txData[0] = counter;
			state = HEADER_RECEIVED;
			break;
    	default:
    		PRINTF("Invalid state.\r\n");
    	}
    	DSPI_SlaveTransferNonBlocking(base, handle, slaveXfer);
    } else {
        PRINTF("Error occurred in this transfer. \r\n\r\n");
    }
}
/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    PRINTF("Hello World\r\n");
    dspi_transfer_t slaveXfer;
    slaveXfer.txData = slaveTxData;
    slaveXfer.rxData = slaveRxData;
    slaveXfer.txData[0] = 0;
    slaveXfer.dataSize = 1;
    slaveXfer.configFlags = kDSPI_SlaveCtar0;
    DSPI_SlaveTransferCreateHandle(SPI0_PERIPHERAL, &g_s_handle, DSPI_SlaveUserCallback, &slaveXfer);

	/* Slave start receive */
    DSPI_SlaveTransferNonBlocking(SPI0_PERIPHERAL, &g_s_handle, &slaveXfer);
    while(1) {
    	if (uart_flag) {
    		uart_flag = false;
    		PRINTF("writing to uart: %d\r\n", uart_data);
    		UART_WriteBlocking(UART3, uart_data, WRITE_SIZE);
    	}
		if (triggered_adc) {
			triggered_adc = false;
			PRINTF("adc read: %d\r\n", adc_value);
			ADC16_SetChannelConfig(ADC1, 0U, &ADC1_channelsConfig[0]);
		}
    }
    return 0 ;
}
