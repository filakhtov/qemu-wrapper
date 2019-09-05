#include <cstddef>

#include "Parser.hh"

const std::string Parser::trimWhitespaces(const std::string& line) const
{
    std::size_t start{line.find_first_not_of("\t ")};
    if (start == std::string::npos) {
        return std::move(std::string{});
    }

    std::size_t end{line.find_last_not_of("\t ")};

    return std::move(line.substr(start, end - start + 1));
}

const std::string Parser::trimComments(const std::string& line) const
{
    std::string trimmed{line};

    std::size_t position{trimmed.find_first_of("#")};
    while (position != std::string::npos) {
        if (position == 0) {
            return std::move(std::string{});
        }

        if (trimmed.at(position - 1) == '\\') {
            if (position == 1) {
                trimmed.erase(--position, 1);
            } else if (trimmed.at(position - 2) == '\\') {
                trimmed.erase(--position, 2);
                break;
            }
        } else {
            break;
        }

        position = trimmed.find_first_of("#", ++position);
    }

    return std::move(trimmed.substr(0, position));
}

const Parameter Parser::lineToParameter(const std::string& line) const
{
    std::size_t name_end{line.find_first_of("\t ")};
    if (name_end == std::string::npos) {
        return Parameter{line};
    }

    std::size_t param_start{line.find_first_not_of("\t ", name_end)};

    return std::move(Parameter{
        line.substr(0, name_end),
        this->parseModifiers(line.substr(param_start))
    });
}

const std::vector<Modifier> Parser::parseModifiers(const std::string& line) const
{
    std::vector<Modifier> modifiers{};

    std::size_t chunk_start{0}, chunk_end{0};
    do {
        chunk_end = line.find_first_of(",", chunk_start);

        modifiers.push_back(this->parseModifier(line.substr(chunk_start, chunk_end - chunk_start)));
        chunk_start = chunk_end + 1;
    } while (chunk_end != std::string::npos);

    return std::move(modifiers);
}

const Modifier Parser::parseModifier(const std::string& line) const
{
    std::size_t name_end{line.find_first_of("=")};
    if (name_end == std::string::npos) {
        return Modifier{line};
    }

    return Modifier{line.substr(0, name_end), line.substr(name_end + 1)};
}

const Parameters Parser::parseStream(std::istream& stream) const
{
    Parameters result{};

    std::string line{};
    while (std::getline(stream, line)) {
        if (!line.size()) {
            continue;
        }

        line = this->trimComments(line);
        if (!line.size()) {
            continue;
        }

        line = this->trimWhitespaces(line);
        if (!line.size()) {
            continue;
        }

        result.push(this->lineToParameter(line));
    }

    return std::move(result);
}
