#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

volatile uint32_t adc_value = 0;
volatile bool ready = true;
volatile bool triggered = false;

/* ADC1_IRQn interrupt handler */
void ADC1_IRQHANDLER(void) {
	if (ADC16_GetChannelStatusFlags(ADC1_PERIPHERAL, 0U) & kADC16_ChannelConversionDoneFlag) {
		ready = true;
		triggered = true;
		adc_value = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, 0U);
	}

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

    bool channel = false;
    while(1) {
		if (ready) {
			ready = false;
			channel = !channel;
			if (channel) {
				ADC16_SetChannelConfig(ADC1, 0U, &ADC1_channelsConfig[0]);
			} else {
				ADC16_SetChannelConfig(ADC1, 0U, &ADC1_channelsConfig[1]);
			}
		}
    	if (triggered) {
    		triggered = false;
			if (channel) {
				PRINTF("channel 1: %d\r\n", adc_value);
				FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_1, kFTM_EdgeAlignedPwm, adc_value * 100.0 / 4096);
			} else {
				PRINTF("channel 5: %d\r\n", adc_value);
				FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_5, kFTM_EdgeAlignedPwm, adc_value * 100.0 / 4096);
			}
			FTM_SetSoftwareTrigger(FTM3, true);
    	}
    }
    return 0 ;
}
