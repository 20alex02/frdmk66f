/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL0_CLK_HZ                         12000000U  /*!< Board xtal0 frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ                          32768U  /*!< Board RTC xtal frequency in Hz */

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************* Configuration BOARD_BootClockHSRUN **********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockHSRUN configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKHSRUN_CORE_CLOCK           180000000U  /*!< Core clock frequency: 180000000Hz */

/*! @brief MCG set for BOARD_BootClockHSRUN configuration.
 */
extern const mcg_config_t mcgConfig_BOARD_BootClockHSRUN;
/*! @brief SIM module set for BOARD_BootClockHSRUN configuration.
 */
extern const sim_clock_config_t simConfig_BOARD_BootClockHSRUN;
/*! @brief OSC set for BOARD_BootClockHSRUN configuration.
 */
extern const osc_config_t oscConfig_BOARD_BootClockHSRUN;

/*******************************************************************************
 * API for BOARD_BootClockHSRUN configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockHSRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************* Configuration BOARD_BootClockVLPR ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockVLPR configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKVLPR_CORE_CLOCK              4000000U  /*!< Core clock frequency: 4000000Hz */

/*! @brief MCG set for BOARD_BootClockVLPR configuration.
 */
extern const mcg_config_t mcgConfig_BOARD_BootClockVLPR;
/*! @brief SIM module set for BOARD_BootClockVLPR configuration.
 */
extern const sim_clock_config_t simConfig_BOARD_BootClockVLPR;
/*! @brief OSC set for BOARD_BootClockVLPR configuration.
 */
extern const osc_config_t oscConfig_BOARD_BootClockVLPR;

/*******************************************************************************
 * API for BOARD_BootClockVLPR configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockVLPR(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */

