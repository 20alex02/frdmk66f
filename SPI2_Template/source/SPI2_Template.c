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
/* TODO: insert other include files here. */
#define TRANSFER_SIZE 64U
#define CMD_WRITE ((uint32_t)0xccU)
#define CMD_READ ((uint32_t)0x55U)

dspi_slave_handle_t g_s_handle;
volatile bool isTransferCompleted = true;
uint8_t slaveRxData[TRANSFER_SIZE] = {0U};
uint8_t slaveTxData[TRANSFER_SIZE] = {0U};
uint8_t ADC_prepared[2];

typedef enum {
	HEADER,
	READ,
	WRITE,
	STATE_COUNT
} State;

void DSPI_SlaveUserCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
	static State state = HEADER;
	static uint8_t counter = 0;
	dspi_transfer_t *slaveXfer = (dspi_transfer_t *) userData;

    if (status == kStatus_Success) {
    	switch (state) {
		case HEADER:
			switch (slaveXfer->rxData[0]) {
			case CMD_READ:
//				slaveXfer->txData = adc value
				state = HEADER;
				break;
			case CMD_WRITE:
				slaveXfer->dataSize = 16;
				state = HEADER;
				break;
			default:
				slaveXfer->txData[0] = counter;
				slaveXfer->dataSize = 1;
			}
			break;
		case READ:
			break;
		case WRITE:
			break;
    	default:
    		PRINTF("Invalid state.\r\n");
    	}
    	DSPI_SlaveTransferNonBlocking(base, handle, slaveXfer);
    } else {
    	// respond to error
        PRINTF("Error occurred in this transfer. \r\n\r\n");
    }

    isTransferCompleted = true;
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

    PRINTF("Hello World\n");
    dspi_transfer_t slaveXfer;
    DSPI_SlaveTransferCreateHandle(SPI0_PERIPHERAL, &g_s_handle, DSPI_SlaveUserCallback, &slaveXfer);
    slaveTxData[0] = 'A';
    slaveTxData[1] = 'C';
    slaveTxData[2] = 'B';

	/* Slave start receive */
    DSPI_SlaveTransferNonBlocking(SPI0_PERIPHERAL, &g_s_handle, &slaveXfer);
    while(1) {
//    	isTransferCompleted   = false;
//    	slaveXfer.txData      = slaveTxData;
//    	slaveXfer.rxData      = slaveRxData;
//    	slaveXfer.dataSize    = 1;
//    	slaveXfer.configFlags = kDSPI_SlaveCtar0;
//
//    	DSPI_SlaveTransferNonBlocking(SPI0_PERIPHERAL, &g_s_handle, &slaveXfer);
//    	while(!isTransferCompleted){}
//    	PRINTF("%x",slaveRxData[0]);
    }
    return 0 ;
}
