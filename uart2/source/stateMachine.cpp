#include "config.hpp"

#include "stateMachine.hpp"

#include "parser.hpp"
#include "stream.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

bool executeAudio(Context& ctx) {
    assert(ctx.state == State::CallMode);
    if (ctx.receivedNotes == ctx.generatedNotes) {
        ctx.debugBuffer.push_back("Audio submitted successfully");
        ctx.audioGenerated = false;
        ctx.receivedNotes.clear();
        ctx.generatedNotes.clear();
        return true;
    }
    ctx.debugBuffer.push_back("Incorrect audio received, restarting call.");
    ctx.receivedNotes.clear();
    return false;
}

std::vector<Note> generateNotes() {
    std::vector<Note> notes(100 + std::rand() % 156);
    for (Note& note : notes) {
        do {
            note.tone = Note::Tone(std::rand() % 12);
            note.octave = Note::Octave(std::rand() % 4);
            note.duration = Note::Duration(std::rand() % 4);
        } while (note.data == static_cast<std::uint8_t>('+'));
    }
    return notes;
}

std::string generateErrorResponse() {
    return "HTTP/1.1 400 Bad Request\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: 0\r\n"
           "\r\n";
}

// encode notes to http response
std::string generateHttpResponse(const std::vector<Note>& notes) {
    std::string notesString;
    for (const auto& note : notes) {
        notesString += note.toString() + "\r\n";
    }

    std::string out = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/plain\r\n"
                      "Content-Length: "
        + std::to_string(notesString.size()) + "\r\n"
                                               "\r\n";
    return out + notesString;
}

bool executeHttpRequest(const HttpRequest& request, Context& ctx, Stream& stream) {
    ctx.debugBuffer.push_back("Http request received:");
    ctx.debugBuffer.push_back("\tMethod: " + request.method);
    ctx.debugBuffer.push_back("\tPath: " + request.path);
    ctx.debugBuffer.push_back("\tHttp version: " + request.http_version);
    ctx.debugBuffer.push_back("\tHeaders:");
    for (const auto& [key, value] : request.headers) {
        ctx.debugBuffer.push_back("\t\t" + key + ": " + value);
    }

    auto it = request.headers.find("Host");
    if (request.method == "GET"
        && request.path == HTTP_REQUEST_PATH
        && request.http_version == "HTTP/1.1"
        && it != request.headers.end()
        && it->second == HTTP_REQUEST_HOST) {
        ctx.generatedNotes = generateNotes();
        stream.print(generateHttpResponse(ctx.generatedNotes));
        ctx.debugBuffer.push_back("Http request processed successfully");
        return true;
    }

    stream.print(generateErrorResponse());
    ctx.debugBuffer.push_back("Incorrect http request received");
    return false;
}

MachineReturnCode runDataMode(Context& ctx, Stream& stream) {
    while (true) {
        auto _c = stream.get<std::uint8_t>();
        if (!_c) {
            return MachineReturnCode::StreamFailure;
        }

        auto res = ctx.httpRequestParser.accept(*_c);
        if (res == ParserReturnCode::Error) {
            ctx.httpRequestParser.reset();
            return MachineReturnCode::ParsingFailure;
        }

        if (res == ParserReturnCode::Escape) {
            ctx.debugBuffer.push_back("Escape sequence received, leaving data mode.");
            ctx.state = State::Connected;
            return MachineReturnCode::Ok;
        }

        if (res == ParserReturnCode::Finished) {
            ctx.debugBuffer.push_back("Http request received:");
            auto request = ctx.httpRequestParser.get();
            ctx.httpRequestParser.reset();
            ctx.audioGenerated = executeHttpRequest(request, ctx, stream);
        }

        // while (s_httpRequestParser.buffered()) {
        //     res = s_httpRequestParser.acceptBuffered();
        //     if (res == ParserReturnCode::Error)
        //         return MachineReturnCode::ParsingFailure;

        //     if (res == ParserReturnCode::Escape)
        //         return MachineReturnCode::IncorrectStateTransition;

        //     if (res == ParserReturnCode::Finished) {
        //         auto request = s_httpRequestParser.get();
        //         ctx.audioGenerated = executeHttpRequest(request, ctx, stream);
        //     }
        // }
    }
}

MachineReturnCode runCallMode(Context& ctx, Stream& stream) {
    while (true) {
        auto _c = stream.get<std::uint8_t>();
        if (!_c)
            return MachineReturnCode::StreamFailure;

        auto res = ctx.audioParser.accept(*_c);
        if (res == ParserReturnCode::Error) {
            ctx.audioParser.reset();
            return MachineReturnCode::ParsingFailure;
        }

        if (res == ParserReturnCode::Escape) {
            ctx.debugBuffer.push_back("Escape sequence received, leaving call mode. Wanted " + std::to_string(ctx.generatedNotes.size()) + " notes, received " + std::to_string(ctx.receivedNotes.size()) + " notes." );
            ctx.state = State::Calling;
            return MachineReturnCode::Ok;
        }

        if (res == ParserReturnCode::Finished) {
            ctx.receivedNotes.push_back(ctx.audioParser.get());
            if (ctx.receivedNotes.size() >= ctx.generatedNotes.size()) {
                ctx.audioCorrect = executeAudio(ctx);
                ctx.audioParser.reset();
                return ctx.audioCorrect ? MachineReturnCode::Ok : MachineReturnCode::IncorrectAudio;
            }

            return MachineReturnCode::Ok;
        }

        // while (g_audioParser.buffered()) {
        //     res = g_audioParser.acceptBuffered();
        //     if (res == ParserReturnCode::Error)
        //         return MachineReturnCode::ParsingFailure;

        //     if (res == ParserReturnCode::Escape)
        //         return MachineReturnCode::BigInternalError;

        //     if (res == ParserReturnCode::Finished) {
        //         ctx.receivedNotes.push_back(g_audioParser.get());
        //         if (ctx.receivedNotes.size() == ctx.generatedNotes.size()) {
        //             ctx.audioCorrect = executeAudio(ctx);
        //             return ctx.audioCorrect ? MachineReturnCode::Ok : MachineReturnCode::IncorrectAudio;
        //         }

        //         return MachineReturnCode::IncorrectAudio;
        //     }
        // }
    }
}

MachineReturnCode runCommand(Context& ctx, Stream& stream) {
    using Type = Command::Type;

    auto _cmd = parseCommand(stream);
    if (!_cmd) {
        stream.println("ERROR syntax");
        return MachineReturnCode::ParsingFailure;
    }

    Command cmd = *_cmd;

    switch (cmd.type) {
    case Type::ATI:
        ctx.debugBuffer.push_back("ATI command received");
        stream.println("Simplified modem simulator");
        stream.println("OK");
        return MachineReturnCode::Ok;

    case Type::ATCGATT:
        ctx.debugBuffer.push_back("ATCGATT command received");
        if (ctx.shouldFail) {
            stream.println("+CGATT: 0");
            stream.println("OK");
        } else {
            if (ctx.state == State::Entry)
                ctx.state = State::NetworkConnected;
            stream.println("+CGATT: 1");
            stream.println("OK");
        }

        return MachineReturnCode::Ok;

    case Type::ATCIICR:
        ctx.debugBuffer.push_back("ATCIICR command received");
        if (ctx.state == State::NetworkConnected) {
            ctx.state = State::GPRSOn;
            stream.println("OK");
            return MachineReturnCode::Ok;
        }

        if (ctx.state == State::GPRSOn) {
            ctx.state = State::NetworkConnected;
            stream.println("OK");
            return MachineReturnCode::Ok;
        }

        stream.println("ERROR");
        return MachineReturnCode::IncorrectStateTransition;

    case Type::ATCIPSTART:
        ctx.debugBuffer.push_back("ATCIPSTART command received with:");
        ctx.debugBuffer.push_back("\tIP := " + cmd.params[0]);
        ctx.debugBuffer.push_back("\tPort := " + cmd.params[1]);
        if (ctx.state == State::GPRSOn) {
            if (cmd.params[0] != "\"" HTTP_REQUEST_IP "\"" || cmd.params[1] != HTTP_REQUEST_PORT) {
                ctx.debugBuffer.push_back("Incorrect IP or port.");
                stream.println("OK");
                stream.println("CONNECT REFUSE");
            } else if (ctx.shouldFail) {
                stream.println("OK");
                stream.println("CONNECT FAIL");
            } else {
                ctx.state = State::Connected;
                stream.println("OK");
                stream.println("CONNECT OK");
            }
            return MachineReturnCode::Ok;
        }

        if (ctx.state == State::Connected) {
            stream.println("ALREADY CONNECT");
            return MachineReturnCode::Ok;
        }

        stream.println("ERROR");
        return MachineReturnCode::IncorrectStateTransition;

    case Type::ATCIPCLOSE:
        ctx.debugBuffer.push_back("ATCIPCLOSE command received");
        if (ctx.state == State::Connected) {
            ctx.state = State::GPRSOn;
            stream.println("OK");
            // ctx.httpRequestParser.reset();
            return MachineReturnCode::Ok;
        }

        stream.println("NO CONNECTION");
        return MachineReturnCode::IncorrectStateTransition;

    case Type::ATO:
        ctx.debugBuffer.push_back("ATO command received");
        if (ctx.state == State::Connected) {
            ctx.state = State::DataMode;
            stream.println("CONNECT");
            return MachineReturnCode::Ok;
        }

        stream.println("ERROR");
        return MachineReturnCode::IncorrectStateTransition;

    case Type::ATD:
        ctx.debugBuffer.push_back("ATD command received with:");
        if (ctx.state == State::NetworkConnected) {
            if (ctx.shouldFail) {
                stream.println("NO CARRIER");
                return MachineReturnCode::Ok;
            }

            ctx.state = State::Calling;
            stream.println("OK");
            return MachineReturnCode::Ok;

        } else {
            stream.println("ERROR");
            return MachineReturnCode::IncorrectStateTransition;
        }
        break;

    case Type::ATH:
        ctx.debugBuffer.push_back("ATH command received");
        if (ctx.state == State::Calling) {
            stream.println("OK");
            ctx.state = State::NetworkConnected;
            return MachineReturnCode::Ok;
        }
        stream.println("ERROR");
        return MachineReturnCode::IncorrectStateTransition;

    case Type::ATC:
        ctx.debugBuffer.push_back("ATC command received");
        if (ctx.state == State::Calling) {
            ctx.state = State::CallMode;
            stream.println("OK");
            return MachineReturnCode::Ok;
        }

        stream.println("NO CALL");
        return MachineReturnCode::IncorrectStateTransition;

    case Type::PlusPlusPlus:
        ctx.debugBuffer.push_back("+++ command received");
        stream.println("OK");
        if (ctx.state == State::DataMode)
            ctx.state = State::Connected;
        else if (ctx.state == State::CallMode)
            ctx.state = State::Calling;
        return MachineReturnCode::Ok;

    case Type::AT:
        ctx.debugBuffer.push_back("AT command received");
        stream.println("OK");
        return MachineReturnCode::Ok;

    case Type::ATRST:
        ctx.debugBuffer.push_back("ATRST command received, reseting device");
        stream.println("OK");
        ctx.state = State::Entry;
        ctx.httpRequestParser.reset();
        ctx.audioParser.reset();
        return MachineReturnCode::Ok;
    }

    return MachineReturnCode::Ok;
}

MachineReturnCode run(Context& ctx, Stream& stream) {
    if (ctx.state == State::DataMode)
        return runDataMode(ctx, stream);

    if (ctx.state == State::CallMode)
        return runCallMode(ctx, stream);

    return runCommand(ctx, stream);
}
