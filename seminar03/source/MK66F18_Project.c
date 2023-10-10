#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

volatile bool timer = false;
volatile bool triggered = false;
volatile uint32_t pin = 0;

void GPIOD_IRQHANDLER(void) {
  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOD);

  triggered = true;
  pin = pin_flags;

  GPIO_PortClearInterruptFlags(GPIOD, pin_flags);

  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

void GPIOA_IRQHANDLER(void) {
  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOA);

  triggered = true;
  pin = pin_flags;

  GPIO_PortClearInterruptFlags(GPIOA, pin_flags);

  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

void PIT_CHANNEL_0_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);

  timer = true;

  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

int main(void) {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\r\n");
    int counter = 0;
    while(1) {
    	if (timer) {
    		++counter;
    		PRINTF("Timer: %d\r\n", counter);
    		timer = false;
    	}
    	if (triggered) {
    		PRINTF("Hello from interrupt, pin: %d\r\n", pin);
    		triggered = false;
    	}
    }
    return 0 ;
}
