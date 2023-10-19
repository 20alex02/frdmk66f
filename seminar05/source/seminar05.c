#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

volatile bool timer = false;


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
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\r\n");
    uint32_t adc0_value;
    uint32_t adc1_value;
    bool ready0 = true;
    bool ready1 = true;
    while(1) {
    	if (timer) {
    		timer = false;
    		if (ready0) {
    			ready0 = false;
    			ADC16_SetChannelConfig(ADC0, 0U, &ADC0_channelsConfig[0]);
    		}
    		if (ready1) {
    			ready1 = false;
    			ADC16_SetChannelConfig(ADC1, 0U, &ADC1_channelsConfig[0]);
    		}
    	}

    	if (ADC16_GetChannelStatusFlags(ADC0, 0U) & kADC16_ChannelConversionDoneFlag) {
    		adc0_value = ADC16_GetChannelConversionValue(ADC0, 0U);
    		ready0 = true;
//    		PRINTF("adc0: %d\r\n", adc0_value);
    	}

    	if (ADC16_GetChannelStatusFlags(ADC1, 0U) & kADC16_ChannelConversionDoneFlag) {
    		adc1_value = ADC16_GetChannelConversionValue(ADC1, 0U);
    		ready1 = true;
//    		PRINTF("adc1: %d\r\n", adc1_value);
    	}
    	PRINTF("adc0: %d, adc1: %d\r\n", adc0_value, adc1_value);
    }
    return 0 ;
}
