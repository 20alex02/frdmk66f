#include "leds.hpp"

#include "MK66F18.h"
#include "board.h"
#include "fsl_common_arm.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

#include <cassert>
#include <cstdint>
#include <thread>
#include <tuple>

using LedData = std::tuple<GPIO_Type*, std::uint32_t>;

static inline LedData getLed(LED led) {
    switch (led) {
    case LED::RGB_Red:
        return { BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN };
    case LED::RGB_Green:
        return { BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN };
    case LED::RGB_Blue:
        return { BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN };
    default:
        assert(false);
    };
    __builtin_unreachable();
}

void setLED(LED led, bool state) {
    auto [gpio, pin] = getLed(led);
    GPIO_PinWrite(gpio, pin, !state);
}

void showColor(Color color) {
    setLED(LED::RGB_Red, color & Red);
    setLED(LED::RGB_Green, color & Green);
    setLED(LED::RGB_Blue, color & Blue);
}

void blink(Color color, unsigned times, std::chrono::microseconds onTime, std::chrono::microseconds offTime) {
    for (unsigned i = 0; i < times; ++i) {
        showColor(color);
        SDK_DelayAtLeastUs(onTime.count(), BOARD_BOOTCLOCKHSRUN_CORE_CLOCK);
        showColor(Black);
        SDK_DelayAtLeastUs(offTime.count(), BOARD_BOOTCLOCKHSRUN_CORE_CLOCK);
    }
}
