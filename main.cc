#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Configuration.hh"
#include "Environment.hh"
#include "Parameters.hh"
#include "Parser.hh"
#include "Executor.hh"
#include "config.hh"

void usage(const std::string& binary_name)
{
    std::cerr << "Usage: " << binary_name << " <config_name>" << std::endl;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> arguments(argv, argv + argc);
    if (arguments.size() != 2) {
        usage(arguments.at(0));

        return EXIT_FAILURE;
    }

    try {
        Configuration c{CONFIGURATION_PATH, CONFIGURATION_EXTENSION, arguments.at(1)};
        const Parser p{};
        const Parameters ps{p.parseStream(c.getStream())};
        const Environment env{"DISPLAY", "TMP", "TMPDIR", "USER", "XAUTHORITY"};
        const Executor exe{QEMU_BINARY_PATH, env, ps};

        exe.execute();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
