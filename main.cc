#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "config.hh"

static std::string binary_name{};

void usage()
{
    std::cerr << "Usage: " << binary_name << " <virtual_machine>" << std::endl;
}

const std::string absolute_path(const std::string& path)
{
    std::unique_ptr<char[]> resolved_path{realpath(path.c_str(), NULL)};
    if (!resolved_path) {
        throw std::invalid_argument{"Invalid configuration path: " + std::string{CONFIGURATION_PATH}};
    }

    return std::string{resolved_path.get()};
}

const std::string get_config_name(const std::string& vm_name)
{
    std::stringstream ss;
    ss << absolute_path(CONFIGURATION_PATH) << "/" << vm_name << ".conf";

    return ss.str();
}

const std::string parse_line(const std::string& line)
{
    std::string option{"-"};
    bool quoted{false};
    char quote{0};
    char previous{0};

    for (const char& c : line) {
        if (quoted) {
            if (c == quote && previous != '\\') {
                quote = 0;
                quoted = false;
            }
        } else {
            if (c == '\'' || c == '"') {
                quote = c;
                quoted = true;
            }
        }

        if (!quoted && c == '#') {
            break;
        }

        option += c;
        previous = c;
    }

    if (quoted) {
        throw new std::invalid_argument{"Malformed line, unbalanced quotes: " + line};
    }

    if (option.size() == 1) {
        option = "";
    }

    return option;
}

const std::vector<std::string> parse_config(std::ifstream& config)
{
    std::vector<std::string> options{};
    std::string line;
    while(std::getline(config, line)) {
        const std::string option{parse_line(line)};

        if (option.size() == 0) {
            continue;
        }

        options.push_back(option);
    }

    return options;
}

const std::vector<std::string> read_config(const std::string& config_file)
{
    std::ifstream config{config_file};
    if (!config.is_open()) {
        throw std::invalid_argument{"Unable to open config file: " + config_file};
    }

    const std::vector<std::string> values{parse_config(config)};

    config.close();

    return values;
}

std::vector<char*> create_arguments(const std::vector<std::string>& options)
{
    std::vector<char*> arguments{};
    arguments.reserve(options.size() + 2);
    arguments.push_back(const_cast<char*>(QEMU_BINARY_PATH));
    for (auto& option : options) {
        arguments.push_back(const_cast<char*>(option.c_str()));
    }
    arguments.push_back(static_cast<char*>(NULL));

    return arguments;
}

std::vector<char*> create_environment()
{
    return std::vector<char*>{
        const_cast<char*>("QEMU_AUDIO_DRV=" QEMU_AUDIO_DRV),
	NULL
    };
}

void execute(const std::vector<std::string>& options)
{
    execve(
        QEMU_BINARY_PATH,
        create_arguments(options).data(),
        create_environment().data()
    );

    char *errorMessage = strerror(errno);
    std::cout << "Execution failed: " << errorMessage << " (" << errno << ")" << std::endl;
}

std::vector<std::string> split_arguments(const std::vector<std::string>& config)
{
    std::vector<std::string> arguments{};
    for (const auto& option : config) {
        auto pos = option.find_first_of(' ');
        if (pos == std::string::npos) {
            arguments.push_back(option);
        } else {
            arguments.push_back(option.substr(0, pos));
            arguments.push_back(option.substr(pos + 1));
        }
    }

    return arguments;
}

void run_qemu(const std::string& vm_name)
{
    execute(split_arguments(read_config(get_config_name(vm_name))));
}

int main(int argc, char* argv[])
{
    std::vector<std::string> arguments(argv, argv + argc);

    binary_name.assign(arguments.at(0));

    if (arguments.size() < 2) {
        std::cerr << "Error: too few arguments." << std::endl << std::endl;
        usage();

        return 1;
    }

    if (arguments.size() > 2) {
        std::cerr << "Error: too many arguments." << std::endl << std::endl;
        usage();

        return 1;
    }

    try {
        run_qemu(arguments.at(1));
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
