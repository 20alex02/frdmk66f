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

volatile uint32_t adc_values[2] = {0};
volatile bool adc_flag = false;
/* ADC1_IRQn interrupt handler */
void ADC1_IRQHANDLER(void) {
  /* Array of result values*/
//  uint32_t result_values[2] = {0};
  /* Get flags for each group */
  for ( int i=0; i<2; i++){
  uint32_t status = ADC16_GetChannelStatusFlags(ADC1_PERIPHERAL, i);
  	if ( status == kADC16_ChannelConversionDoneFlag){
  		adc_values[i] = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, i);
  	}
  }

  /* Place your code here */
  adc_flag = true;

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
//    uint32_t adc1_value;
//    bool ready = true;
    bool led_color = false;
    while(1) {
    	if (timer) {
    		timer = false;
    		led_color = !led_color;
        	FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_5, kFTM_EdgeAlignedPwm, led_color ? 50 : 0);
        	FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_1, kFTM_EdgeAlignedPwm, led_color ? 0 : 50);
        	FTM_SetSoftwareTrigger(FTM3, true);
    	}

//    	if (adc_flag) {
//    		adc_flag = false;
//    	}
//    	if (timer) {
//    		timer = false;
//    		if (ready) {
//    			ready = false;
//    			ADC16_SetChannelConfig(ADC1, 0U, &ADC1_channelsConfig[0]);
//    		}
//    	}
//
//    	if (ADC16_GetChannelStatusFlags(ADC1, 0U) & kADC16_ChannelConversionDoneFlag) {
//    		adc1_value = ADC16_GetChannelConversionValue(ADC1, 0U);
//    		ready = true;
////    		PRINTF("adc1: %d\r\n", adc1_value);
//    	}
//    	PRINTF("0: %d, 1: %d\r\n", adc_values[0], adc_values[1]);

    }
    return 0 ;
}
