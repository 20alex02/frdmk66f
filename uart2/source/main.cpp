#include "config.hpp"

#include "buttons.hpp"
#include "leds.hpp"
#include "parser.hpp"
#include "stateMachine.hpp"
#include "stream.hpp"

#include "MK66F18.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "peripherals.h"
#include "pin_mux.h"

#include <cassert>
#include <iostream>
#include <span>

void display(const State& state) {
    switch (state) {
    case State::Entry:
        showColor(Black);
        break;

    case State::NetworkConnected: // fallthrough
    case State::GPRSOn:
        showColor(Red);
        break;

    case State::Connected:
        showColor(Yellow);
        break;

    case State::Calling:
        showColor(Magenta);
        break;

    case State::DataMode:
        showColor(Green);
        break;

    case State::CallMode:
        showColor(Blue);
        break;
    };
}


void releaseMain() {
#if USE_DEBUG_STREAM
    DebugStream stream;
#else
    UARTStream stream(UART1);
#endif
    Context context;

    while (1) {
        // SW2 is used to simulate module errors
        if (readSW(Button::SW2)) {
            std::cout << "SW2 pressed, the next command will fail" << std::endl;
            context.shouldFail = true;
        }
        MachineReturnCode res = run(context, stream);
        switch (res) {
        case MachineReturnCode::Ok:
            break;

        case MachineReturnCode::BigInternalError:
            std::cerr << "Big internal error" << std::endl;
            break;

        case MachineReturnCode::IncorrectAudio:
            std::cerr << "Incorrect audio" << std::endl;
            break;

        case MachineReturnCode::IncorrectHttpRequest:
            std::cerr << "Incorrect HTTP request" << std::endl;
            break;

        case MachineReturnCode::IncorrectStateTransition:
            std::cerr << "Incorrect state transition" << std::endl;
            break;

        case MachineReturnCode::ParsingFailure:
            std::cerr << "Parsing failure" << std::endl;
            break;

        case MachineReturnCode::StreamFailure:
            std::cerr << "Stream failure" << std::endl;
            break;

        }

        display(context.state);

        while (!context.debugBuffer.empty()) {
            std::cout << context.debugBuffer.front() << "\r" << std::endl;
            context.debugBuffer.pop_front();
        }

        context.shouldFail = false;
    }
}

int main(void) {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif
#if RUN_TESTS
    testMain();
#endif
#if RUN_INTERACTIVE
    releaseMain();
#endif
    while (1)
        __asm__("nop");
    return 0;
}
