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


unsigned int run_intcode_program(std::vector<unsigned int> numbers) {
    for (std::vector<int>::size_type i = 0; i < numbers.size() - 4; i += 4) {
        auto opcode = numbers[i];
        if (opcode == 99) {
            // Done
            break;
        } else if (opcode != 1 && opcode != 2 ) {
            std::cerr << "Unknown opcode: " << opcode << std::endl;
            exit(3);
        }
        auto input_index_a = numbers[i+1];
        auto input_index_b = numbers[i+2];
        auto output_index = numbers[i+3];
        if (numbers.size() < input_index_a || numbers.size() < input_index_b
            || numbers.size() < output_index) {
            std::cerr << "Index out of range" << std::endl;
            exit(4);
        }

        int result = -1;
        if (opcode == 1) {
            // Addition
            result = numbers[input_index_a] + numbers[input_index_b];
        } else if (opcode == 2) {
            // Multiplication
            result = numbers[input_index_a] * numbers[input_index_b];
        }
        numbers[output_index] = result;
    }
    return numbers[0];
}
