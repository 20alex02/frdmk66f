#pragma once

#include "stream.hpp"

#include <cassert>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

struct Command {
    enum class Type {
        PlusPlusPlus,
        ATI,
        ATO,
        ATCGATT,
        ATCIICR,
        ATCIPSTART,
        ATCIPCLOSE,
        ATD,
        ATH,
        ATC,
        AT,
        ATRST,
    } type;
    std::vector<std::string> params;

    Command(Type type = Type::AT, std::vector<std::string> params = {})
        : type(type)
        , params(params) {
    }
};

static inline std::ostream& operator<<(std::ostream& os, const Command::Type& type) {
    switch (type) {
    case Command::Type::PlusPlusPlus:
        os << "+++";
        break;
    case Command::Type::ATI:
        os << "ATI";
        break;
    case Command::Type::ATO:
        os << "ATO";
        break;
    case Command::Type::ATCGATT:
        os << "AT+CGATT";
        break;
    case Command::Type::ATCIICR:
        os << "AT+CIICR";
        break;
    case Command::Type::ATCIPSTART:
        os << "AT+CIPSTART";
        break;
    case Command::Type::ATCIPCLOSE:
        os << "AT+CIPCLOSE";
        break;
    case Command::Type::ATD:
        os << "ATD";
        break;
    case Command::Type::ATH:
        os << "ATH";
        break;
    case Command::Type::ATC:
        os << "ATC";
        break;
    case Command::Type::AT:
        os << "AT";
        break;
    case Command::Type::ATRST:
        os << "AT+RST";
        break;
    }
    return os;
}

union Note {
    enum Octave : std::uint8_t {
        O3 = 0,
        O4 = 1,
        O5 = 2,
        O6 = 3,
    };

    enum Tone : std::uint8_t {
        C = 0,
        C_, // C sharp
        D,
        D_, // D sharp
        E,
        F,
        F_, // F sharp
        G,
        G_, // G sharp
        A,
        A_, // A sharp
        B,
    };

    enum Duration : std::uint8_t {
        Whole = 0,
        Half = 1,
        Quarter = 2,
        Eighth = 3,
    };

    struct {
        Octave octave : 2;
        Tone tone : 4;
        Duration duration : 2;
    };

    std::uint8_t data;

    Note() = default;
    Note(std::uint8_t data)
        : data(data) {
    }

    bool operator==(const Note& other) const { return data == other.data; }

    std::string toString() const {
        static const std::vector<std::string> toneToString = {
            "C", // Note::C
            "C#", // Note::C_
            "D", // Note::D
            "D#", // Note::D_
            "E", // Note::E
            "F", // Note::F
            "F#", // Note::F_
            "G", // Note::G
            "G#", // Note::G_
            "A", // Note::A
            "A#", // Note::A_
            "B", // Note::B
        };

        static const std::vector<std::string> octaveToString = {
            "3", // Note::O3
            "4", // Note::O4
            "5", // Note::O5
            "6", // Note::O6
        };

        static const std::vector<std::string> durationToString = {
            "1", // Note::Whole
            "1/2", // Note::Half
            "1/4", // Note::Quarter
            "1/8", // Note::Eighth
        };

        return toneToString.at(tone) + octaveToString.at(octave)
            + " : " + durationToString.at(duration);
    }
};

static inline std::ostream& operator<<(std::ostream& os, const Note& note) {
    return os << note.toString() << " (" << std::hex << std::showbase << static_cast<int>(note.data) << std::dec << ")";
}

struct HttpRequest {
    std::string method;
    std::string path;
    std::string http_version;
    std::map<std::string, std::string> headers;
};

std::optional<Command> parseCommand(Stream& stream);

enum class ParserReturnCode {
    Error = -1,
    NotFinished = 0,
    Finished = 1,
    Escape,
};

static inline std::ostream& operator<<(std::ostream& os, const ParserReturnCode& code) {
    switch (code) {
    case ParserReturnCode::Error:
        os << "ParserReturnCode::Error";
        break;
    case ParserReturnCode::NotFinished:
        os << "ParserReturnCode::NotFinished";
        break;
    case ParserReturnCode::Finished:
        os << "ParserReturnCode::Finished";
        break;
    case ParserReturnCode::Escape:
        os << "ParserReturnCode::Escape";
        break;
    }
    return os;
}

// template <typename Parser, typename Result>
// concept ParserConcept = requires(const Parser& parser, std::uint8_t c) {
//     { parser.isReady() } -> std::same_as<bool>;
// } && requires(Parser& parser) {
//     { parser.accept(c) } -> std::same_as<ParserReturnCode>;
//     { parser.get() } -> std::same_as<Result>;
//     { parser.reset() } -> std::same_as<void>;
// };

// For now, we only support GET requests
class HttpRequestParser {
private:
    enum State {
        Method,
        Path,
        HTTPVersion,
        HTTPVersionEatNewline,
        HeaderKeyStart,
        HeaderKey,
        HeaderKeyEatSpace,
        HeaderValue,
        HeaderValueEatNewline,
        EatNewline,
        Finished,
        Error,
    };

    State m_state = State::Method;

    HttpRequest m_request;

    std::vector<std::uint8_t> key;
    std::vector<std::uint8_t> value;

public:
    using Result = HttpRequest;

    HttpRequestParser() {
        key.reserve(32);
        value.reserve(32);
    }

    ParserReturnCode accept(std::uint8_t c) {
        switch (m_state) {
        case State::Method:
            if (c == ' ')
                m_state = State::Path;
            else
                m_request.method += c;
            break;
        case State::Path:
            if (c == ' ')
                m_state = State::HTTPVersion;
            else
                m_request.path += c;
            break;
        case State::HTTPVersion:
            if (c == '\r')
                m_state = State::HTTPVersionEatNewline;
            else
                m_request.http_version += c;

            break;
        case State::HTTPVersionEatNewline:
            if (c == '\n')
                m_state = State::HeaderKeyStart;
            else
                m_state = State::Error;

            break;
        case State::HeaderKeyStart:
            if (c == '\r')
                m_state = State::EatNewline;
            else {
                m_state = State::HeaderKey;
                key.push_back(c);
            }
            break;
        case State::HeaderKey:
            if (c == ':')
                m_state = State::HeaderKeyEatSpace;
            else
                key.push_back(c);

            break;
        case State::HeaderKeyEatSpace:
            if (c == ' ')
                m_state = State::HeaderValue;
            else
                m_state = State::Error;

            break;
        case State::HeaderValue:
            if (c == '\r') {
                m_state = State::HeaderValueEatNewline;
                m_request.headers.insert({ std::string(key.begin(), key.end()),
                    std::string(value.begin(), value.end()) });
            } else
                value.push_back(c);

            break;
        case State::HeaderValueEatNewline:
            if (c == '\n') {
                m_state = State::HeaderKeyStart;
                key.clear();
                value.clear();
            } else
                m_state = State::Error;

            break;
        case State::EatNewline:
            if (c == '\n')
                m_state = State::Finished;
            else
                m_state = State::Error;
            break;
        case State::Finished:
        case State::Error: // fallthrough
            return ParserReturnCode::Error;
        }

        switch (m_state) {
            case State::Finished:
                return ParserReturnCode::Finished;
            case State::Error:
                return ParserReturnCode::Error;
            default:
                return ParserReturnCode::NotFinished;
        }
        __builtin_unreachable();
    }

    Result get() {
        assert(m_state == State::Finished);
        m_state = State::Method;
        return m_request;
    }

    void reset() {
        m_state = State::Method;
        m_request = {};
    }

    bool isReady() const { return m_state == State::Finished; }
};

class AudioParser {
private:
    bool m_isReady = false;
    Note m_note;

public:
    using Result = Note;

    ParserReturnCode accept(std::uint8_t c) {
        assert(!m_isReady);
        m_note.data = c;
        m_isReady = true;
        return ParserReturnCode::Finished;
    }

    Result get() {
        assert(m_isReady);
        m_isReady = false;
        return m_note;
    }

    void reset() { m_isReady = false; }

    bool isReady() const { return m_isReady; }
};

template <typename Parser>
class EscapingWrapper {
private:
    Parser m_inner;
    const std::vector<std::uint8_t> m_escapeSequence;

    std::list<std::uint8_t> m_buffer;

public:
    using Result = typename Parser::Result;

    EscapingWrapper(Parser&& parser, std::vector<std::uint8_t> escapeSequence)
        : m_inner(std::move(parser))
        , m_escapeSequence(escapeSequence) {
    }

    EscapingWrapper(std::vector<std::uint8_t> escapeSequence)
        : m_escapeSequence(escapeSequence) {
        static_assert(std::is_default_constructible_v<Parser>);
    }

    ParserReturnCode accept(std::uint8_t c) {
        if (c == m_escapeSequence[m_buffer.size()]) {
            m_buffer.push_back(c);
            if (m_buffer.size() == m_escapeSequence.size()) {
                m_buffer.clear();
                return ParserReturnCode::Escape;
            }
            return ParserReturnCode::NotFinished;
        }

        if (m_buffer.empty())
            return m_inner.accept(c);

        m_buffer.push_back(c);
        return acceptBuffered();
    }

    ParserReturnCode acceptBuffered() {
        while (!m_buffer.empty()) {
            auto res = m_inner.accept(m_buffer.front());
            m_buffer.pop_front();
            if (res != ParserReturnCode::NotFinished)
                return res;
        }
        return ParserReturnCode::NotFinished;
    }

    std::size_t buffered() const { return m_buffer.size(); }

    bool isReady() const { return m_inner.isReady(); }

    Result get() {
        assert(m_inner.isReady());
        return m_inner.get();
    }

    void reset() {
        m_inner.reset();
        m_buffer.clear();
    }
};
