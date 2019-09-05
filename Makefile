CC = g++
CXXFLAGS = -std=c++11 -pedantic -pedantic-errors -Wall -Weffc++ -Wextra -Wcast-align -Wcast-qual -Wconversion \
    -Wdisabled-optimization -Werror -Wfloat-equal -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k \
    -Wimport -Winit-self -Winvalid-pch -Wmissing-field-initializers -Wmissing-format-attribute  -Wmissing-include-dirs \
    -Wmissing-noreturn -Wpacked -Wpadded -Wpointer-arith -Wredundant-decls -Wshadow -Wstack-protector \
    -Wstrict-aliasing=2 -Wswitch-default -Wswitch-enum -Wunreachable-code -Wunused -Wunused-parameter \
    -Wvariadic-macros -Wwrite-strings

#ifdef DEBUG
CXXFLAGS += -g -O
#else
CXXFLAGS += -O2
#endif

export CXXFLAGS CC

.PHONY: all clean test

all: qemu

clean:
	rm -vf *.o qemu

qemu: Configuration.o Executor.o Environment.o Parameters.o Parser.o main.o
	$(CXX) -o $@ $^

main.o: main.cc config.hh Parameters.hh Parser.hh Configuration.hh
Parameters.o: Parameters.cc Parameters.hh
Parser.o: Parser.cc Parser.hh
Environment.o: Environment.cc Environment.hh
Executor.o: Executor.cc Executor.hh
Configuration.o: Configuration.cc Configuration.hh
