#ifndef PARAMETERS_HH
#define PARAMETERS_HH

#include <stdexcept>
#include <string>
#include <vector>

class Modifier
{
private:
    const std::string name;
    const std::string value;
public:
    Modifier(const std::string&);

    Modifier(const std::string&, const std::string&);

    operator std::string(void) const;
};

class Parameter
{
private:
    const std::string name;
    std::vector<Modifier> modifiers;

    std::string modifiersToString(void) const;
public:
    Parameter(const std::string&, const std::vector<Modifier>&);

    Parameter(const std::string&);

    std::string getName(void) const noexcept;

    bool hasValue(void) const noexcept;

    std::string getValue(void) const;
};

class Parameters
{
private:
    std::vector<Parameter> parameters;
public:
    Parameters(void);

    void push(const Parameter&);

    const std::vector<Parameter>& getParameters(void) const noexcept;
};

#endif // PARAMETERS_HH
