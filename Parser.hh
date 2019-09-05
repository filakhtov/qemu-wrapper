#ifndef PARSER_HH
#define PARSER_HH

#include <istream>
#include <string>

#include "Parameters.hh"

class Parser
{
private:
    const std::string trimWhitespaces(const std::string&) const;

    const std::string trimComments(const std::string&) const;

    const Parameter lineToParameter(const std::string&) const;

    const std::vector<Modifier> parseModifiers(const std::string&) const;

    const Modifier parseModifier(const std::string&) const;

public:
    const Parameters parseStream(std::istream&) const;
};

#endif // PARSER_HH
