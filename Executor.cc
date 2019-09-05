#include "Executor.hh"

#include <string.h>
#include <unistd.h>

Executor::Executor(const std::string& bin_path, const Environment& e, const Parameters& p) :
    binary_path{bin_path}, environment{e}, parameters{p}
{
}

const std::vector<char*> Executor::prepareArguments(void) const
{
    std::vector<char*> arguments{strdup(this->binary_path.c_str())};

    for (const auto& parameter : this->parameters.getParameters()) {
        const std::string arg_name{parameter.getName()};
        arguments.push_back(strdup(arg_name.c_str()));

        if (parameter.hasValue()) {
            const std::string arg_value{parameter.getValue()};
            arguments.push_back(strdup(arg_value.c_str()));
        }
    }

    arguments.push_back(NULL);

    return std::move(arguments);
}

const std::vector<char*> Executor::prepareEnvironment(void) const
{
    std::vector<char*> variables{};

    for (const auto& variable : this->environment.getEnvironmentVariables()) {
        variables.push_back(strdup(static_cast<std::string>(variable).c_str()));
    }

    variables.push_back(NULL);

    return std::move(variables);
}

void Executor::execute(void) const
{
    const std::vector<char*>& args{this->prepareArguments()};
    const std::vector<char*>& env{this->prepareEnvironment()};

    execve(this->binary_path.c_str(), args.data(), env.data());
}
