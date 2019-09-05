# qemu-wrapper

This is a simple wrapper for QEMU written in C++. The idea is to have a directory (repository of virtual machines), that contains `.conf` (extension can be configured using `CONFIGURATION_EXTENSION` macro) configuration files with virtual machine definitions and be able to start any machine by running the `qemu <machine-name>` command.

### Building and Installing
Copy the `config.hh.in` file into the `config.hh` file and adjust as necessary. Execute the `make` command to build the `qemu` binary and then place it whenever you like (e.g. `/opt/bin`, `/usr/local/bin`).

### Configuration file format

#### Format
Configuration file is just a simple set command-line options that are expected by the `qemu` command, without a leading `-` (dash) symbol, one option per line. First word on each line is an option name, such as `drive` for `-drive` option or `cpu` for `-cpu` option. Then follows an optional value for the given option, such as `file=ubuntu.iso,media=cdrom` for `-drive file=ubuntu.iso,media=cdrom` option or `Ubuntu Linux` for `-name "Ubuntu Linux"` option. Note, that option value is read as is and there is no need to use quotes `"` even if value contains spaces.

#### Comments
Comments can be added using `#`, either in between the line or at the end of the line. If there is a need to use a `#` (hash) anywhere in the value it can be prefixed by the `\` (backslash) to escape it. In an unlikely event where `\` (backslash) is needed right befor the `#` (hash) symbol it can be prefixed by another backslash, i.e. `\\# comment`, this will produce just the `\` (backslash) value.

### Contributions
Just open up a PR or raise an issue and let's work from there.
