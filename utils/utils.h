#include <fstream>
#include <vector>

std::ifstream open_input_file(int argc, char **argv);

unsigned int run_intcode_program(std::vector<unsigned int> numbers);
