#ifndef ENVIRONMENT_HH
#define ENVIRONMENT_HH

#include <stdexcept>
#include <string>
#include <vector>

class EnvironmentVariable
{
private:
    const std::string name;
    const std::string value;
public:
    EnvironmentVariable(const std::string&, const std::string&);

    const std::string& getName(void) const noexcept;

    const std::string& getValue(void) const noexcept;

    operator std::string(void) const;
};

class EnvironmentVariablesNotUnique : public std::runtime_error
{
private:
    std::string createMessage(const std::vector<std::string>&) const;
public:
    EnvironmentVariablesNotUnique(const std::vector<std::string>&);
};

class Environment
{
private:
    const std::vector<EnvironmentVariable> variables;

    void ensureUnique(const std::vector<std::string>&) const;

    const std::vector<EnvironmentVariable> collectEnvVars(const std::vector<std::string>&) const;
public:
    Environment(const std::initializer_list<std::string>&);

    const std::vector<EnvironmentVariable>& getEnvironmentVariables(void) const noexcept;
};

#endif // ENVIRONMENT_HH
