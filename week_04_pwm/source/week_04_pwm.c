#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
#include "HSV_RGB.h"

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    int h = 0;
    RgbColor rgb;
    while(1) {
    	rgb = HSVtoRGB(h, 100, 100);

    	FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_5, kFTM_EdgeAlignedPwm, rgb.r * 100 / 255);
    	FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_1, kFTM_EdgeAlignedPwm, rgb.g * 100 / 255);
    	TPM_UpdatePwmDutycycle(TPM2, kTPM_Chnl_1, kTPM_EdgeAlignedPwm, rgb.b * 100 / 255);
    	FTM_SetSoftwareTrigger(FTM3, true);

    	h = (h + 1) % 360;
    	SDK_DelayAtLeastUs(50 * 1000, SystemCoreClock);
    }
    return 0 ;
}
