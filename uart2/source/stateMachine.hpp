#pragma once

#include "parser.hpp"
#include "stream.hpp"

#include <iostream>
#include <list>
#include <string>

enum class State {
    Entry,
    NetworkConnected,
    GPRSOn,
    Connected,
    Calling,
    DataMode,
    CallMode,
};

static inline std::ostream& operator<<(std::ostream& os, const State& state) {
    switch (state) {
        case State::Entry:
            os << "State::Entry";
            break;
        case State::NetworkConnected:
            os << "State::NetworkConnected";
            break;
        case State::GPRSOn:
            os << "State::GPRSOn";
            break;
        case State::Connected:
            os << "State::Connected";
            break;
        case State::Calling:
            os << "State::Calling";
            break;
        case State::DataMode:
            os << "State::DataMode";
            break;
        case State::CallMode:
            os << "State::CallMode";
            break;
    }
    return os;
}

enum class MachineReturnCode {
    BigInternalError = -6, // Should not happen => unknown internal error
    StreamFailure = -5, // Internal error - hw possibly
    IncorrectHttpRequest = -4, // incorrect http request submitted
    IncorrectAudio = -3, // incorrect audio submitted
    ParsingFailure = -2, // incorrect syntax submitted
    IncorrectStateTransition = -1, // incorrect command submitted
    Ok = 0, // No error to report
};

static inline std::ostream& operator<<(std::ostream& os, const MachineReturnCode& code) {
    switch (code) {
        case MachineReturnCode::BigInternalError:
            os << "BigInternalError";
            break;
        case MachineReturnCode::StreamFailure:
            os << "StreamFailure";
            break;
        case MachineReturnCode::IncorrectHttpRequest:
            os << "IncorrectHttpRequest";
            break;
        case MachineReturnCode::IncorrectAudio:
            os << "IncorrectAudio";
            break;
        case MachineReturnCode::ParsingFailure:
            os << "ParsingFailure";
            break;
        case MachineReturnCode::IncorrectStateTransition:
            os << "IncorrectStateTransition";
            break;
        case MachineReturnCode::Ok:
            os << "Ok";
            break;
    }
    return os;
}

struct Context {
    State state = State::Entry;
    bool shouldFail = false;
    std::vector<Note> generatedNotes;
    std::vector<Note> receivedNotes;
    bool audioGenerated = false;
    bool audioCorrect = false;
    std::list<std::string> debugBuffer;
    EscapingWrapper<HttpRequestParser> httpRequestParser { { '+', '+', '+' } };
    EscapingWrapper<AudioParser> audioParser { { '+', '+', '+' } };
};

MachineReturnCode runCommand(Context& ctx, Stream& stream);
MachineReturnCode runAudio(Context& ctx, Stream& stream);
MachineReturnCode runHttpRequest(Context& ctx, Stream& stream);

MachineReturnCode run(Context& ctx, Stream& stream);
