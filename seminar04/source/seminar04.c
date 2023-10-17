#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

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
//    TPM_UpdatePwmDutycycle(TPM2, kTPM_Chnl_1, kTPM_EdgeAlignedPwm, 0);
    int i = 0 ;
    while(1) {
    	SDK_DelayAtLeastUs(10 * 1000, SystemCoreClock);
//    	TPM_UpdatePwmDutycycle(TPM2, kTPM_Chnl_1, kTPM_EdgeAlignedPwm, i);
//    	PRINTF("main loop %d\r\n", i);
    	FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_1, kFTM_EdgeAlignedPwm, i);
//    	i = (i + 1) % 101;
    	++i;

    }
    return 0 ;
}
