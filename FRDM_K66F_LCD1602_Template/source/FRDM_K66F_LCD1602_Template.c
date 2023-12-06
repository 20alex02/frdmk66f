/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
#include "lcd.h"
#include "fsl_rtc.h"


volatile int direction = 0;
//volatile Direction direction = EAST;
/* PORTA_IRQn interrupt handler */
void GPIOA_IRQHANDLER(void) {
  /* Get pin flags */
  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOA);

  /* Place your interrupt code here */
  direction = (direction + 1) % 4;

  /* Clear pin flags */
  GPIO_PortClearInterruptFlags(GPIOA, pin_flags);

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/* PORTD_IRQn interrupt handler */
void GPIOD_IRQHANDLER(void) {
  /* Get pin flags */
  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOD);

  /* Place your interrupt code here */
  direction = (direction - 1) % 4;

  /* Clear pin flags */
  GPIO_PortClearInterruptFlags(GPIOD, pin_flags);

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    // Initialize the LCD
    LCD_4BitsInit(BOARD_BOOTCLOCKHSRUN_CORE_CLOCK, false, false);
    char point[] = ".";
    int x = 0;
    int y = 0;
    PRINTF("hello\r\n");
    // main loop of the demo application
    while (1)
      {
    	PRINTF("loop\r\n");
    	SDK_DelayAtLeastUs(1000 * 100);
    	switch (direction) {
    	case (0):
    		++x;
    		break;
    	case (2):
    		--x;
    		break;
    	case (1):
    	case (3):
    		y = y == 0 ? 1 : 0;
    		break;
    	default:
    		x = y = 0;
    		break;
    	}
    	x %= 16;

  		// display date and time
  		LCD_SetPosition(x, y);
  		PRINTF("x: %d, y: %d\r\n", x, y);
  		LCD_Print(point);
      }
}


//typedef enum {
//	WELCOME,
//	COUNTDOWN,
//	RESET,
//	RESULT,
//	STATE_COUNT
//} State;
//
//
//volatile bool sw2_pressed = false;
//volatile bool second_elapsed = false;
//volatile uint32_t miliseconds = 0;
//
///* PORTD_IRQn interrupt handler */
//void GPIOD_IRQHANDLER(void) {
//  /* Get pin flags */
//  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOD);
//
//  /* Place your interrupt code here */
//  sw2_pressed = true;
//
//  /* Clear pin flags */
//  GPIO_PortClearInterruptFlags(GPIOD, pin_flags);
//
//  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
//     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
//  #if defined __CORTEX_M && (__CORTEX_M == 4U)
//    __DSB();
//  #endif
//}
//
///* PIT0_IRQn interrupt handler */
//void PIT_CHANNEL_0_IRQHANDLER(void) {
//  uint32_t intStatus;
//  /* Reading all interrupt flags of status register */
//  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
//  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);
//
//  /* Place your code here */
//  second_elapsed = true;
//
//  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
//     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
//  #if defined __CORTEX_M && (__CORTEX_M == 4U)
//    __DSB();
//  #endif
//}
//
///* PIT1_IRQn interrupt handler */
//void PIT_CHANNEL_1_IRQHANDLER(void) {
//  uint32_t intStatus;
//  /* Reading all interrupt flags of status register */
//  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_1);
//  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_1, intStatus);
//
//  /* Place your code here */
//  ++miliseconds;
//
//  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
//     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
//  #if defined __CORTEX_M && (__CORTEX_M == 4U)
//    __DSB();
//  #endif
//}
//
//
//int main(void) {
//    /* Init board hardware. */
//    BOARD_InitBootPins();
//    BOARD_InitBootClocks();
//    BOARD_InitBootPeripherals();
//    BOARD_InitDebugConsole();
//
//    // Initialize the LCD
//    LCD_4BitsInit(BOARD_BOOTCLOCKHSRUN_CORE_CLOCK, false, false);
//
//    State state = WELCOME;
//
//    char msg[] = "pressSw2AfterYou";
//    char msg2[] = "see 0. next->sw2";
//
//    char reset_msg[] = "pressedTooEarly";
//
//    char time[2] = {0};
//    uint8_t countdown = 3;
//
//    char result[] = "yourReactionTime";
//    char miliseconds_msg[12] = {0};
//
//    while (1) {
//    	if (sw2_pressed) {
//    		sw2_pressed = false;
//    		switch (state) {
//    		case WELCOME:
//    			state = COUNTDOWN;
//    			countdown = 3;
//    			PIT_StartTimer(PIT, kPIT_Chnl_0);
//    			break;
//    		case COUNTDOWN:
//    			if (countdown > 0) {
//    				state = RESET;
//    				PIT_StopTimer(PIT, kPIT_Chnl_0);
//                    LCD_SetPosition(0, 0);
//                    LCD_Print(reset_msg);
//    			} else {
//    				state = RESULT;
//    				PIT_StopTimer(PIT, kPIT_Chnl_1);
//    			    sprintf(miliseconds_msg, "%u", miliseconds);
//    			    miliseconds = 0;
//                    LCD_SetPosition(0, 0);
//                    LCD_Print(result);
//                    LCD_SetPosition(0, 1);
//                    LCD_Print(miliseconds_msg);
//    			}
//    			break;
//    		case RESET:
//    		case RESULT:
//    		default:
//    			state = WELCOME;
//                LCD_SetPosition(0, 0);
//                LCD_Print(msg);
//                LCD_SetPosition(0, 1);
//                LCD_Print(msg2);
//    		}
//    	}
//    	if (second_elapsed) {
//    		second_elapsed = false;
//    		time[0] = '0' + countdown;
//    		if (--countdown == 0) {
//    			PIT_StopTimer(PIT, kPIT_Chnl_0);
//    			PIT_StartTimer(PIT, kPIT_Chnl_1);
//    		}
//            LCD_SetPosition(0, 0);
//            LCD_Print(time);
//    	}
//    }
//}
