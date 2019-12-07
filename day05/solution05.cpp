#include <sstream>

#include "utils.h"


int main(int argc, char **argv) {
    auto input_stream = open_input_file(argc, argv);
    auto numbers = load_intcode_program(input_stream);

    std::stringstream program_input;

    std::cout << "PART 1" << std::endl;
    std::cout << "Test program output for ID 1: ";
    program_input = std::stringstream{"1"};
    run_intcode_program(numbers, program_input, std::cout);
    std::cout << std::endl;
    std::cout << "PART 2" << std::endl;
    std::cout << "Test program output for ID 5: ";
    program_input = std::stringstream{"5"};
    run_intcode_program(numbers, program_input, std::cout);
    return 0;
}
