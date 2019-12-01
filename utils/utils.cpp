#include <iostream>

#include "utils.h"


std::ifstream open_input_file(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " input.txt" << std::endl;
        exit(1);
    }

    std::ifstream input_stream(argv[1]);
    if (!input_stream) {
        std::cerr << "File not found: " << argv[1] << std::endl;
        exit(2);
    }

    return input_stream;
}
