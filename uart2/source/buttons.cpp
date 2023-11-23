#include "buttons.hpp"

#include "MK66F18.h"
#include "board.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

#include <tuple>
#include <cassert>
#include <cstdint>

using ButtonData = std::tuple<GPIO_Type*, std::uint32_t>;

static inline ButtonData getButton(Button btn) {
    switch(btn) {
        case Button::SW2:
            return {BOARD_SW2_GPIO, BOARD_SW2_PIN};
        case Button::SW3:
            return {BOARD_SW3_GPIO, BOARD_SW3_PIN};
        default:
            assert(false);
    };
    __builtin_unreachable();
}

bool readSWClick(Button btn) {
    auto [base, pin] = getButton(btn);
    if(GPIO_PinRead(base, pin))
        return false;
    
    while(!GPIO_PinRead(base, pin))
        asm("nop");
}

bool readSW(Button btn) {
    auto [base, pin] = getButton(btn);
    return !GPIO_PinRead(base, pin);
}