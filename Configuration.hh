#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <fstream>
#include <istream>

class Configuration
{
private:
    std::ifstream stream;
public:
    Configuration(const std::string&, const std::string&, const std::string&);

    std::istream& getStream(void) noexcept;
};

#endif
