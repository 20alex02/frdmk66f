/*
 * Copyright 2016-2019 NXP
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
 * @file    MK66FN2M0xxx18_Accelerometer.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

#include "fsl_fxos.h"
#include "fsl_gpio.h"
#include "math.h"
#include "accel.h"

#define SENSOR_ADDRESS 0x1DU
#define DEFAULT_DATA_SCALE 2U
#define BUFFER_SIZE 6

volatile bool orientation_changed = false;
/* PORTC_IRQn interrupt handler */
void GPIOC_IRQHANDLER(void) {
  /* Get pin flags */
  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOC);

  /* Place your interrupt code here */
  orientation_changed = true;

  /* Clear pin flags */
  GPIO_PortClearInterruptFlags(GPIOC, pin_flags);

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


status_t setupOrientationDetection() {
	uint8_t tmp;
	status_t status = BOARD_Accel_I2C_Receive(SENSOR_ADDRESS, PL_CFG_REG, 1, &tmp, 1);
	if (status != kStatus_Success) {
		return status;
	}
	tmp |= PL_EN_MASK;
	status = BOARD_Accel_I2C_Send(SENSOR_ADDRESS, PL_CFG_REG, 1, tmp);
	if (status != kStatus_Success) {
		return status;
	}

	status = BOARD_Accel_I2C_Receive(SENSOR_ADDRESS, CTRL_REG4, 1, &tmp, 1);
	if (status != kStatus_Success) {
		return status;
	}
	tmp |= INT_EN_LNDPRT_MASK;
	status = BOARD_Accel_I2C_Send(SENSOR_ADDRESS, CTRL_REG4, 1, tmp);
	if (status != kStatus_Success) {
		return status;
	}
    return kStatus_Success;
}

/**
 * Configures FXOS8700CQ accelerometer.
 */
status_t initializeAccel() {
    fxos_handle_t fxosHandle = {0};
    fxos_config_t config     = {0};
    status_t result          = kStatus_Fail;

    /* Configure the I2C function */
    config.I2C_SendFunc    = BOARD_Accel_I2C_Send;
    config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;
    config.slaveAddress = SENSOR_ADDRESS;

    /* Initialize accelerometer sensor */
    result = startInitialization(&fxosHandle, &config);
    if (result != kStatus_Success)
    {
        PRINTF("\r\nSensor device initialization start failed!\r\n");
        return result;
    }

    result = setupOrientationDetection();
    if (result != kStatus_Success) {
    	PRINTF("\r\nOrientation detection initialization failed!\r\n");
    	return result;
    }

    result = finishInitialization(&fxosHandle, &config);
    if (result != kStatus_Success) {
    	PRINTF("\r\nSensor device initialize finish failed!\r\n");
    	return result;
    }
    return result;
}

void print_orientation(uint8_t orientation) {
	if (orientation & LAPO1_MASK && orientation & LAPO0_MASK) {
		PRINTF("landscape left\r\n");
	} else if (orientation & LAPO1_MASK) {
		PRINTF("landscape right\r\n");
	} else if (orientation & LAPO0_MASK) {
		PRINTF("portrait down\r\n");
	} else {
		PRINTF("portrait up\r\n");
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
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("PV198 - 6. lecture - I2C accelerometer\r\n");

    if (initializeAccel() != kStatus_Success) {
    	PRINTF("\r\nInitialization failed!\r\n");
    	return -1;
    }
    uint8_t orientation = 0;
    while(1) {
    	if (orientation_changed) {
    		orientation_changed = false;
    		if (BOARD_Accel_I2C_Receive(SENSOR_ADDRESS, PL_STATUS_REG, 1, &orientation, 1) != kStatus_Success) {
        		PRINTF("receive failed\r\n");
        		continue;
        	}
    		print_orientation(orientation);
    	}
//    	SDK_DelayAtLeastUs(1000 * 100);
    }
    return 0;
}
