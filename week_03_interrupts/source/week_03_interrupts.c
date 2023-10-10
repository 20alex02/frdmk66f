#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

volatile bool sw3_pressed = false;
volatile bool timer = false;

/* PORTA_IRQn interrupt handler */
void GPIOA_IRQHANDLER(void) {
  /* Get pin flags */
  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOA);

  /* Place your interrupt code here */
  sw3_pressed = true;

  /* Clear pin flags */
  GPIO_PortClearInterruptFlags(GPIOA, pin_flags);

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/* PIT0_IRQn interrupt handler */
void PIT_CHANNEL_0_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);

  /* Place your code here */
  timer = true;

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    PIT_StopTimer(PIT, 0);
    while(1) {
    	if (sw3_pressed) {
    		PIT_StartTimer(PIT, 0);
    		sw3_pressed = false;
    	}
    	if (timer) {
    		PIT_StopTimer(PIT, 0);
    		timer = false;
    		if (!GPIO_PinRead(GPIOA, 10)) {
    			PRINTF("still pressed\r\n");
    		}
    	}
    }
    return 0 ;
}
