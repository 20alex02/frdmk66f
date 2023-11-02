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


status_t setupOrientationDetection();

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

status_t setupOrientationDetection() {
	// TODO: setup accelerometer to generate an interrupt when orientation changes
    return kStatus_Success;
}

/*
 * @brief   Application entry point.
 */

// TODO for hw - PL_STATUS, PL_CFG, CTRL_REG4
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
    uint8_t landscape;
    uint8_t mask = 1 << 7;
    while(1) {
    	if (orientation_changed) {
    		orientation_changed = false;
    		PRINTF("orientation changed\r\n");
    		if (BOARD_Accel_I2C_Receive(SENSOR_ADDRESS, PL_STATUS_REG, 1, landscape, 1) != kStatus_Success) {
        		PRINTF("receive failed\r\n");
        		continue;
        	}
    		if (landscape & mask) {
    			PRINTF("orientation changed\r\n");
    		}

    	}

//    	BOARD_Accel_I2C_Send(SENSOR_ADDRESS, OUT_X_MSB_REG, 1, )
//    	if (BOARD_Accel_I2C_Receive(SENSOR_ADDRESS, OUT_X_MSB_REG, 1, buf, buf_size) != kStatus_Success) {
//    		PRINTF("receive failed\r\n");
//    	}
//    	x = (buf[0] << 6) | (buf[1] >> 2);
//    	if (BOARD_Accel_I2C_Receive(SENSOR_ADDRESS, OUT_Y_MSB_REG, 1, buf, buf_size) != kStatus_Success) {
//    		PRINTF("receive failed\r\n");
//    	}
//    	y = (buf[0] << 6) | (buf[1] >> 2);
//    	if (BOARD_Accel_I2C_Receive(SENSOR_ADDRESS, OUT_Z_MSB_REG, 1, buf, buf_size) != kStatus_Success) {
//    		PRINTF("receive failed\r\n");
//    	}
//    	z = (buf[0] << 6) | (buf[1] >> 2);

//    	PRINTF("x: %6d y:%6d z:%6d\r\n", x, y, z);
    	SDK_DelayAtLeastUs(1000 * 100);
    }
    return 0;
}
