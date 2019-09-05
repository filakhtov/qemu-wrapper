#include <fstream>
#include <stdexcept>

#include "Configuration.hh"

Configuration::Configuration(const std::string& path, const std::string& extension, const std::string& name) :
    stream{}
{
    const std::string full_path{path + "/" + name + "." + extension};

    this->stream.open(full_path);
    if (this->stream.fail()) {
        throw std::runtime_error{"Unable to open the requested configuration file: " + full_path};
    }
}

std::istream& Configuration::getStream(void) noexcept
{
    return this->stream;
}
