#include <fstream>
#include <istream>
#include <vector>

std::ifstream open_input_file(int argc, char **argv);

std::vector<unsigned int> load_intcode_program(std::istream &input_stream);

unsigned int run_intcode_program(std::vector<unsigned int> numbers);
