/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
#include "lcd.h"
#include "fsl_rtc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
// definition of positions on the screen
#define DAY_POS 6
#define MONTH_POS 9
#define YEAR_POS 12
#define HOUR_POS 6
#define MINUTE_POS 9
#define SECOND_POS 12
#define ZERO_CHAR_VAL 48

/* RTC second interrupt handler */
bool update_rdt = true;

void RTC_1_SECONDS_IRQHANDLER(void) {
	update_rdt = true;
}

void set_two_digit(char* msg, unsigned pos, unsigned val)
{
	msg[pos] = '0' + val / 10;
	msg[pos+1] = '0' + val % 10;
}

void set_four_digit(char* msg, unsigned pos, unsigned val)
{
	set_two_digit(msg, pos, val / 100);
	set_two_digit(msg, pos+2, val % 100);
}

// 0.56 V
int main(void)
{
	// date and time strings
    char date_str[] = "date: DD.MM.YYYY";
    char time_str[] = "time: HH.MM.SS";
    // date and time configuration structure used by the RTC driver
    rtc_datetime_t rdt;

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    // Initialize the LCD
    LCD_4BitsInit(BOARD_BOOTCLOCKHSRUN_CORE_CLOCK, false, false);

    // main loop of the demo application
    while (1)
      {
  		// get current date and time
  		RTC_GetDatetime(RTC_1_PERIPHERAL, &rdt);
  		// convert date and time to strings
  		set_two_digit(date_str, DAY_POS, rdt.day);
  		set_two_digit(date_str, MONTH_POS, rdt.month);
  		set_four_digit(date_str, YEAR_POS, rdt.year);
  		set_two_digit(time_str, HOUR_POS, rdt.hour);
  		set_two_digit(time_str, MINUTE_POS, rdt.minute);
  		set_two_digit(time_str, SECOND_POS, rdt.second);
  		// display date and time
  		LCD_SetPosition(0, 0);
  		LCD_Print(date_str);
  		LCD_SetPosition(0, 1);
  		LCD_Print(time_str);
  		// wait for next second
  		__WFI();

      }
}
