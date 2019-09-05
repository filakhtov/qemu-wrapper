#ifndef EXECUTOR_HH
#define EXECUTOR_HH

#include <string>

#include "Environment.hh"
#include "Parameters.hh"

class Executor
{
private:
    const std::string binary_path;
    const Environment environment;
    const Parameters parameters;

    const std::vector<char*> prepareArguments(void) const;

    const std::vector<char*> prepareEnvironment(void) const;
public:
    Executor(const std::string&, const Environment&, const Parameters&);

    void execute(void) const;
};

#endif // EXECUTOR_HH
