#include <fstream>
#include <istream>
#include <vector>

std::ifstream open_input_file(int argc, char **argv);


enum class Opcode {
    ADD = 1,
    MULTIPLY = 2,
    END = 99
};

Opcode int_to_opcode(int integer);

std::vector<int> load_intcode_program(std::istream &input_stream);

int run_intcode_program(std::vector<int> numbers);
