#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
#include <stdbool.h>

void delay(void) {
    volatile uint32_t i = 0;
	for (i = 0; i < 800000; ++i) {
        __asm("NOP"); /* delay */
    }
}

void toggle_led(int color){
    if (color == 0) {
        GPIO_PortToggle(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    } else if (color == 1) {
        GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);
    } else {
    	GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);
    }
}

int main(void) {

    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, &led_config);
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);

    int color = 0;
    bool sw2, sw3;
    bool sw2_prev = true, sw3_prev = true;
    while (1) {
    	sw2 = GPIO_PinRead(GPIOD, 11);
    	sw3 = GPIO_PinRead(GPIOA, 10);
    	if (!sw2 && sw2_prev) {
    		delay();
    		if (!GPIO_PinRead(GPIOD, 11)) {
    			++color;
    			color %= 3;
    		}
    	}
    	if (!sw3 && sw3_prev) {
    		delay();
    		if (!GPIO_PinRead(GPIOA, 10)) {
        		toggle_led(color);
    		}
    	}
    	sw2_prev = sw2;
    	sw3_prev = sw3;
    }
    return 0;
}
