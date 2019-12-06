#include <iostream>
#include <string>

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


Opcode int_to_opcode(int integer) {
    switch(integer) {
        case 1:
            return Opcode::ADD;
        case 2:
            return Opcode::MULTIPLY;
        case 99:
            return Opcode::END;
        default:
            std::cerr << "Unknown opcode: " << integer << std::endl;
            exit(3);
    }
}


std::vector<int> load_intcode_program(std::istream &input_stream) {
    std::vector<int> numbers;
    std::string num_str;
    while (std::getline(input_stream, num_str, ',')) {
        numbers.push_back(std::stoi(num_str));
    }
    return numbers;
}


int run_intcode_program(std::vector<int> numbers) {
    for (std::vector<int>::size_type i = 0; i < numbers.size() - 4; i += 4) {
        auto opcode = int_to_opcode(numbers[i]);
        switch (opcode) {
            case Opcode::END:
                i = numbers.size();
                break;
            case Opcode::ADD:
            case Opcode::MULTIPLY:
            {
                // 3 operands
                auto input_index_a = numbers[i+1];
                auto input_index_b = numbers[i+2];
                auto output_index = numbers[i+3];
                if (   input_index_a < 0 || int(numbers.size()) <= input_index_a
                    || input_index_b < 0 || int(numbers.size()) <= input_index_b
                    || output_index  < 0 || int(numbers.size()) <= output_index) {
                    std::cerr << "Index out of range" << std::endl;
                    exit(4);
                }
                int result = -1;
                switch (opcode) {
                    case Opcode::ADD:
                        result = numbers[input_index_a] + numbers[input_index_b];
                        break;
                    case Opcode::MULTIPLY:
                        result = numbers[input_index_a] * numbers[input_index_b];
                        break;
                    default:
                        std::cerr << "Unexpected opcode: " << int(opcode) << std::endl;
                        exit(3);
                }
                numbers[output_index] = result;
                break;
            }
            default:
                std::cerr << "Unexpected opcode: " << int(opcode) << std::endl;
                exit(3);
        }
    }
    return numbers[0];
}
