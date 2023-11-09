/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_uart.h"
#include "fsl_clock.h"
#include "fsl_ftm.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* Definition of peripheral ID */
#define UART3_PERIPHERAL UART3
/* Definition of the clock source frequency */
#define UART3_CLOCK_SOURCE CLOCK_GetFreq(UART3_CLK_SRC)
/* Definition of peripheral ID */
#define FTM2_PERIPHERAL FTM2
/* Definition of the clock source frequency */
#define FTM2_CLOCK_SOURCE CLOCK_GetFreq(kCLOCK_BusClk)
/* Definition of the clock source frequency */
#define FTM2_TIMER_MODULO_VALUE (((FTM2_CLOCK_SOURCE/ (1U << (FTM2_PERIPHERAL->SC & FTM_SC_PS_MASK))) / 262144) - 1)
/* Definition of peripheral ID */
#define FTM3_PERIPHERAL FTM3
/* Definition of the clock source frequency */
#define FTM3_CLOCK_SOURCE CLOCK_GetFreq(kCLOCK_BusClk)
/* Definition of the clock source frequency */
#define FTM3_TIMER_MODULO_VALUE (((FTM3_CLOCK_SOURCE/ (1U << (FTM3_PERIPHERAL->SC & FTM_SC_PS_MASK))) / 262144) - 1)

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const uart_config_t UART3_config;
extern const ftm_config_t FTM2_config;
extern const ftm_config_t FTM3_config;

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
