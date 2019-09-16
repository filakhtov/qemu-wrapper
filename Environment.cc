#include <algorithm>
#include <cstdlib>

#include "Environment.hh"

using string_vector = std::vector<std::string>;


EnvironmentVariable::EnvironmentVariable(const std::string& n, const std::string& v) : name{n}, value{v}
{
}

const std::string& EnvironmentVariable::getName(void) const noexcept
{
    return this->name;
}

const std::string& EnvironmentVariable::getValue(void) const noexcept
{
    return this->value;
}

EnvironmentVariable::operator std::string(void) const
{
    return std::move(this->name + "=" + this->value);
}

EnvironmentVariablesNotUnique::EnvironmentVariablesNotUnique(const string_vector& names) :
    std::runtime_error{this->createMessage(names)}
{
}

std::string EnvironmentVariablesNotUnique::createMessage(const string_vector& names) const
{
    std::string message{"Duplicate environment variables: "};
    for (auto const& name : names) {
        message += name + ", ";
    }

    return std::move(std::string{message.begin(), message.end() - 2});
}

Environment::Environment(const std::initializer_list<std::string>& var_names) :
    variables{this->collectEnvVars(var_names)}
{
}

const std::vector<EnvironmentVariable> Environment::collectEnvVars(const string_vector& var_names) const
{
    std::vector<EnvironmentVariable> vars;
    vars.reserve(var_names.size());
    for (const auto& name : var_names) {
        auto env_value{std::getenv(name.c_str())};
        if (env_value == NULL) {
            continue;
        }
        const std::string value{env_value};

        vars.push_back({name, value});
    }

    return std::move(vars);
}

void Environment::ensureUnique(const string_vector& vars) const
{
    string_vector temp{vars};
    std::sort(temp.begin(), temp.end());
    auto last = std::unique(temp.begin(), temp.end());
    auto wasUnique = (last == temp.end());

    if (!wasUnique) {
        throw EnvironmentVariablesNotUnique{vars};
    }
}

const std::vector<EnvironmentVariable>& Environment::getEnvironmentVariables(void) const noexcept
{
    return this->variables;
}
