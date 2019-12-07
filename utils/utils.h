#include <fstream>
#include <istream>
#include <iostream>
#include <ostream>
#include <vector>

std::ifstream open_input_file(int argc, char **argv);

enum class Opcode {
    ADD = 1,
    MULTIPLY = 2,
    INPUT = 3,
    OUTPUT = 4,
    JUMP_TRUE = 5,
    JUMP_FALSE = 6,
    LESS_THAN = 7,
    EQUALS = 8,
    END = 99
};

Opcode int_to_opcode(int integer);

enum class Mode {
    POSITIONAL = 0,
    IMMEDIATE = 1
};

std::vector<Mode> int_to_modes(int integer, int num_operands);

std::vector<int> load_intcode_program(std::istream &input_stream);

int run_intcode_program(std::vector<int> numbers,
                        std::istream &input = std::cin,
                        std::ostream &output = std::cout);
