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

std::string trim(const std::string& str)
{
    if (str.size() == 0) {
        return str;
    }

    std::size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) {
        return str;
    }

    std::size_t last = str.find_last_not_of("\t ");

    return str.substr(first, (last - first + 1));
}

const std::string parse_line(const std::string& line)
{
    if (line.size() == 0 || line.at(0) == '#') {
        return std::string{};
    }

    std::string trimmed_line{trim(line)};
    std::string option{"-"};
    std::size_t pos{0}, start_at{0};
    while((pos = trimmed_line.find_first_of('#', pos))) {
        if (pos == std::string::npos) {
            option += trimmed_line.substr(start_at);
            break;
        }

        if (trimmed_line.at(pos - 1) != '\\') {
            option += trimmed_line.substr(start_at, pos);
            break;
        }

        option += trimmed_line.substr(start_at, pos - 1);
        option += '#';
        start_at = ++pos;
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
        auto pos = option.find_first_of(" \t");
        if (pos == std::string::npos) {
            arguments.push_back(option);
        } else {
            arguments.push_back(option.substr(0, pos));

            pos = option.find_first_not_of(" \t", pos);

            arguments.push_back(option.substr(pos));
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
