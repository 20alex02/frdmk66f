#include "parser.hpp"

#include "stream.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

static inline std::optional<std::string> startsWith(const std::string& full, const std::string& sub) {
    if (full.size() < sub.size())
        return std::nullopt;

    auto left = full.begin();
    auto right = sub.begin();

    while (right != sub.end())
        if (*left++ != *right++)
            return std::nullopt;
    return full.substr(sub.size());
}

std::optional<Command> parseCommand(Stream& stream) {
    Command command;
    std::string line;
    while (true) {
        auto _line = stream.readUntil('\n');
        if (!_line) {
            return std::nullopt;
        }
        if (_line->empty())
            continue;

        line = std::string(_line->begin(), _line->end());
        break;
    }

    // std::cout << "parsing: " << line << "\r" << std::endl;

    if (line == "+++") { // +++\n
        command.type = Command::Type::PlusPlusPlus;
        command.params = {};

    } else if (line == "ATI") { // ATI\n
        command.type = Command::Type::ATI;
        command.params = {};

    } else if (line == "ATO") { // ATO\n
        command.type = Command::Type::ATO;
        command.params = {};

    } else if (line == "AT+CGATT?") { // AT+CGATT?\n
        command.type = Command::Type::ATCGATT;
        command.params = {};

    } else if (line == "AT+CIICR") { // AT+CIICR\n
        command.type = Command::Type::ATCIICR;
        command.params = {};

    } else if (auto _params = startsWith(line, "AT+CIPSTART=")) { // AT+CIPSTART="<IP>",<PORT>\n
        command.type = Command::Type::ATCIPSTART;
        command.params = {};
        std::stringstream params(*_params);
        std::string param;

        std::getline(params, param, ',');
        command.params.push_back(param);

        std::getline(params, param);
        command.params.push_back(param);

    } else if (line == "AT+CIPCLOSE") { // AT+CIPCLOSE\n
        command.type = Command::Type::ATCIPCLOSE;
        command.params = {};

    } else if (auto _params = startsWith(line, "ATD+")) { // ATD+<NUMBER>\n
        command.type = Command::Type::ATD;
        command.params = { *_params };

    } else if (line == "ATH") { // ATH\n
        command.type = Command::Type::ATH;
        command.params = {};

    } else if (line == "ATC") { // ATC\n
        command.type = Command::Type::ATC;
        command.params = {};
    } else if (line == "AT") { // AT\n
        command.type = Command::Type::AT;
        command.params = {};
    } else if (line == "AT+RST") {
        command.type = Command::Type::ATRST;
        command.params = {};
    } else {
        return std::nullopt;
    }

    return command;
}
