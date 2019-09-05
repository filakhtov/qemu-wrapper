#include "Parameters.hh"

Modifier::Modifier(const std::string& n) : name{n}, value{}
{
}

Modifier::Modifier(const std::string& n, const std::string& v) : name{n}, value{v}
{
}

Modifier::operator std::string(void) const
{
    if (this->value.size()) {
        return std::move(this->name + "=" + this->value);
    }

    return this->name;
}

std::string Parameter::modifiersToString(void) const
{
    if (!this->modifiers.size()) {
        return "";
    }

    std::string result{};

    for (const auto& modifier : this->modifiers) {
        result += modifier;
        result += ",";
    }

    return std::move(std::string{result.cbegin(), result.cend() - 1});
}

Parameter::Parameter(const std::string& n, const std::vector<Modifier>& m) : name{n}, modifiers{m}
{
}

Parameter::Parameter(const std::string& n) : name{n}, modifiers{}
{
}

std::string Parameter::getName(void) const noexcept
{
    return std::move(std::string{"-" + this->name});
}

bool Parameter::hasValue(void) const noexcept
{
    return this->modifiers.size() > 0;
}

std::string Parameter::getValue(void) const
{
    return std::move(this->modifiersToString());
}

Parameters::Parameters(void) : parameters{}
{
}

void Parameters::push(const Parameter& p)
{
    this->parameters.push_back(p);
}

const std::vector<Parameter>& Parameters::getParameters(void) const noexcept
{
    return this->parameters;
}
