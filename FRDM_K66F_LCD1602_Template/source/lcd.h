#include "fsl_gpio.h"
#include "pin_mux.h"

#define LCD_FREQ 22000000// 18MHz

#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

typedef enum {
	LCD_SEND_COMMAND,
	LCD_SEND_CHAR
} MessageType_t;

/*!
 * @brief Initialization of the LCD display 2 x 16 charcaters, 4 bits interface connection.
 *
 * This function initializes the LCD display 2 x 16 characters in four bits interface connection.
 * It use the Systick timer for timing of initialization and commands.
 *
 * @param core_clk_freq   Clock frequency that is used for driving Systick timer (usually core clock frequency).
 * @param cursor_on   Enable/disable the cursor on the display.
 * @param cursor_blinking   Enable/disable blinking of the cursor on the display.
 */
void LCD_4BitsInit(uint32_t systick_clk_freq, bool cursor_on, bool cursor_blinking);

void LCD_SendData(uint8_t data, MessageType_t messageType);

void LCD_SendCommand(uint8_t command);

void LCD_PutChar(uint8_t Character);

void LCD_Print(char s[]);

void LCD_Clear();

void LCD_Home();

void LCD_SetPosition(uint8_t x, uint8_t y);

#endif
