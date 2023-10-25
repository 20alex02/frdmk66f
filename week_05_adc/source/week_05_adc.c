#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

volatile uint32_t adc_value = 0;
volatile bool triggered = false;

/* ADC1_IRQn interrupt handler */
void ADC1_IRQHANDLER(void) {
	if (ADC16_GetChannelStatusFlags(ADC1_PERIPHERAL, 0U) & kADC16_ChannelConversionDoneFlag) {
		triggered = true;
		adc_value = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, 0U);
	}

	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
	 Store immediate overlapping exception return operation might vector to incorrect interrupt. */
	#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
	#endif
}

//volatile bool timer = false;
///* PIT0_IRQn interrupt handler */
//void PIT_CHANNEL_0_IRQHANDLER(void) {
//  uint32_t intStatus;
//  /* Reading all interrupt flags of status register */
//  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
//  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);
//
//  /* Place your code here */
//  timer = true;
//
//  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
//     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
//  #if defined __CORTEX_M && (__CORTEX_M == 4U)
//    __DSB();
//  #endif
//}


int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    bool red = false;
    bool ready = true;
    while(1) {
		if (ready) {
			ready = false;
			red = !red;
			ADC16_SetChannelConfig(ADC1, 0U, &ADC1_channelsConfig[red ? 0 : 1]);
		}
    	if (triggered) {
    		triggered = false;
    		ready = true;
    		FTM_UpdatePwmDutycycle(FTM3, red ? kFTM_Chnl_5 : kFTM_Chnl_1, kFTM_EdgeAlignedPwm, adc_value * 100 / 4096);
			FTM_SetSoftwareTrigger(FTM3, true);
    	}
//    	if (timer) {
//    		timer = false;
//    		PRINTF("%5s, value: %4d, percentage: %3d\r\n", red ? "red" : "green", adc_value, adc_value * 100 / 4096);
//    	}
    }
    return 0 ;
}
