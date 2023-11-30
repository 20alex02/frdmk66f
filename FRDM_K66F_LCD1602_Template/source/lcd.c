/*
 * LCD 1602A driver supporting 4 data signal interface
 * This driver supports the HD44780U Dot Matrix Liquid Crystal Display Controller.
 */


#include "lcd.h"
#include <string.h>


// delays for command execution
#define LCD_DATA_DELAY 		1 	// 1 ms
#define LCD_HOME_CMD_DELAY 	2	// 2 ms (Home command execution time is 1.52ms)
#define LCD_START_UP_DELAY 	15 	// 15 ms
#define LCD_INIT_DELAY 		5 	// 5 ms


#define LCD_DISPLAY_MODE_4BITS 0x2C  // 4 bits interface, 2 lines display, 5x10 dots

#define LCD_DISPLAY_CONTROL 				0x8
#define LCD_DISPLAY_CONTROL_DISPLAY_ON 		0x4
#define LCD_DISPLAY_CONTROL_CURSOR 			0x2
#define LCD_DISPLAY_CONTROL_CURSOR_BLINKING 0x1

#define LCD_COMMAND_CLEAR 					0x1
#define LCD_COMMAND_HOME  					0x2
#define LCD_COMMAND_SET_ADDR 				0x80
#define LCD_COMMAND_SET_ADDR_2_LINE_OFFSET 	0x40

// Output values of pins
#define SET_OUTPUT 1
#define RESET_OUTPUT 0

// number of SysTick timer ticks for one millisecond (initialized by the LCD_4BitsInit() function)
static uint32_t lcd_systick_ms = 0;


/* Function set the delay by using the load value of the systick timer. The counter flag is set when the timer reach zero (after the set delay). */

/*!
 * @brief Function set the delay by using the load value of the systick timer.
 * The counter flag is set when the timer reach zero (after the delay).
 *
 * @note This function does not block the execution of the application.
 *
 * @param time_ms - delay time in milliseconds
 */
static void LCD_SetDelay(uint16_t time_ms)
{

  SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->LOAD  = (uint32_t)(lcd_systick_ms * time_ms - 1UL);      /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
				   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick and SysTick CPU clock source*/
}

/*!
 * @brief Function that provides waiting for the specified time in internal loop
 *
 * @param time_ms - delay time in milliseconds
 */
static void LCD_Delay(uint16_t time_ms)
{
	LCD_SetDelay(time_ms);
	// wait until timeout
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
};


/********* function for LC 8bit initilize **************/
/*!
 * @brief Initialization of the LCD display controller by using 4 bits data interface
 *
 * @note The SysTick timer is used by this driver to provide data/command execution delays.
 *
 * @param systick_clk_freq - input reference clock of the Systick timer (usually the CPU core clock frequency)
 * @param cursor_on - enabled/disable the cursor on the screen
 * @param cursor_blinking - enable/disable the blinking of the cursor on the screen
 */
void LCD_4BitsInit(uint32_t systick_clk_freq, bool cursor_on, bool cursor_blinking)
{
	// Set one millisecond resolution of the Systick timer counter
	lcd_systick_ms = systick_clk_freq / 1000;

	// 4 bits (wire) interface initialization sequence
	LCD_Delay(LCD_START_UP_DELAY);
	LCD_SendCommand(0x33);//
	LCD_Delay(LCD_INIT_DELAY);
	LCD_SendCommand(0x32);//
	LCD_Delay(LCD_INIT_DELAY);
	// Send command to LCD for initialize LCD as 4 bit
	LCD_SendCommand(LCD_DISPLAY_MODE_4BITS);
	// Send command to LCD for Display ON, and cursor setting
	LCD_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_CONTROL_DISPLAY_ON | (cursor_on ? LCD_DISPLAY_CONTROL_CURSOR : 0) | (cursor_blinking ? LCD_DISPLAY_CONTROL_CURSOR_BLINKING : 0));
	// Send command to LCD for clear Display and set the first position
	LCD_Clear();
	LCD_SetPosition(0, 0);
};

/************ send command to LCD *************/
/*!
 * @brief Send command to display controller
 *
 * @param command - command ID that will be send
 */
void LCD_SendCommand(uint8_t command)
{
	LCD_SendData(command, LCD_SEND_COMMAND);// Send the command to the LCD
};

/*!
 * @brief Send one character on the display at the current cursor position.
 *
 * @param character - character that will be displayed
 */
void LCD_PutChar(uint8_t character)
{
	LCD_SendData(character, LCD_SEND_CHAR);// Send the character that displays on the LCD
};

/*!
 * @brief Send string on the display (sequence of characters on the active row)
 *
 * @note The position of the display is not checked. It must be ensured that the string is not
 * longer than the available space on the active row of the display.
 *
 * @param s - string to be displayed
 */
void LCD_Print(char s[])
{
	uint8_t str_len = strlen(s); // variable i == number length string
	uint8_t c;
	// Send all characters
	for(c = 0;c < str_len;c++){
		LCD_PutChar(s[c]); // send character
	}
};

/*!
 * @brief Clear the content of the display
 *
 */
void LCD_Clear()
{
	LCD_SendCommand(LCD_COMMAND_CLEAR); // clear LCD character
};

/*!
 * @brief Set the home position of the cursor (0,0).
 *
 */
void LCD_Home()
{
	LCD_SendCommand(LCD_COMMAND_HOME); // return Home
	LCD_SetDelay(LCD_HOME_CMD_DELAY);
};

/*!
 * @brief Set cursor position on the display where the next character will be displayed.
 *
 * @param x - column of the display screen (range 0 - 15)
 * @param y - row of the display screen (range 0 - 1)
 */
void LCD_SetPosition(uint8_t x, uint8_t y)
{
	if ((y == 0) && ( (x >= 0) && (x < 16) ) ){
		LCD_SendCommand(LCD_COMMAND_SET_ADDR | x);
	}
	else if (y == 1 && ( (x >= 0 && x < 16) ) ){
		LCD_SendCommand( (LCD_COMMAND_SET_ADDR | (x + LCD_COMMAND_SET_ADDR_2_LINE_OFFSET) ));
	}
};


/*!
 * @brief Sending of the data by using the control and data signals that are routed to output pins
 *
 * @note This function use the following definitions of routed signals to output pins:
 * LCD_RS_GPIO,  LCD_RS_PIN
 * LCD_RW_GPIO,  LCD_RW_PIN
 * LCD_E_GPIO, 	 LCD_E_PIN
 * LCD_DB4_GPIO, LCD_DB4_PIN
 * LCD_DB5_GPIO, LCD_DB5_PIN
 * LCD_DB6_GPIO, LCD_DB6_PIN
 * LCD_DB7_GPIO, LCD_DB7_PIN
 *
 *
 * @param data - 8 bits data or command that are sent to LCD controller
 * @param messageType - selection between data or command transfer (LCD_SEND_CHAR or LCD_SEND_COMMAND)
 */

void LCD_SendData(uint8_t data, MessageType_t messageType)
{
	// wait until minimum delay after the last command/data transfer
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	// TODO: Set RS pin to 0 or 1
	GPIO_PinWrite(GPIOA, LCD_RS_PIN, messageType == LCD_SEND_COMMAND ? RESET_OUTPUT : SET_OUTPUT);

	// TODO: Reset the RW pin
	GPIO_PinWrite(GPIOA, LCD_RW_PIN, RESET_OUTPUT);

	// TODO: prepare DATA to send - set up GPIO pins according to data being send (one GPIO = one bit from "data")
	GPIO_PinWrite(GPIOA, LCD_DB7_PIN, data & (1 << 7));
	GPIO_PinWrite(GPIOA, LCD_DB6_PIN, data & (1 << 6));
	GPIO_PinWrite(GPIOA, LCD_DB5_PIN, data & (1 << 5));
	GPIO_PinWrite(GPIOA, LCD_DB4_PIN, data & (1 << 4));

	// TODO: Enable the enable pin
	GPIO_PinWrite(GPIOA, LCD_E_PIN, SET_OUTPUT);

	// wait 1 ms
	LCD_Delay(LCD_DATA_DELAY);
	// TODO: Disable the enable pin
	GPIO_PinWrite(GPIOA, LCD_E_PIN, RESET_OUTPUT);

	// wait 1 ms
	LCD_Delay(LCD_DATA_DELAY);

	// TODO: prepare DATA to send - set up GPIO pins according to data being send (one GPIO = one bit from "data")
	GPIO_PinWrite(GPIOA, LCD_DB7_PIN, data & (1 << 3));
	GPIO_PinWrite(GPIOA, LCD_DB6_PIN, data & (1 << 2));
	GPIO_PinWrite(GPIOA, LCD_DB5_PIN, data & (1 << 1));
	GPIO_PinWrite(GPIOA, LCD_DB4_PIN, data & (1 << 0));

	// TODO: Enable the enable pin
	GPIO_PinWrite(GPIOA, LCD_E_PIN, SET_OUTPUT);

	// wait 1 ms
	LCD_Delay(LCD_DATA_DELAY);
	// TODO: Disable the enable pin
	GPIO_PinWrite(GPIOA, LCD_E_PIN, RESET_OUTPUT);

	//GPIO_PortClear(LCD_RS_GPIO,1 << LCD_RS_PIN);
	// Set the minimum delay for the next command
	LCD_SetDelay(LCD_DATA_DELAY);
}
