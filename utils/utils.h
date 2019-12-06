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
    END = 99
};

Opcode int_to_opcode(int integer);

std::vector<int> load_intcode_program(std::istream &input_stream);

int run_intcode_program(std::vector<int> numbers,
                        std::istream &input = std::cin,
                        std::ostream &output = std::cout);
