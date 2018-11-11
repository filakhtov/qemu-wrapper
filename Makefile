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

qemu: main.o
	$(CXX) -o qemu main.o

main.o: config.hh main.cc
