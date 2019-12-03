#include <iostream>
#include <string>
#include <vector>

#include "utils.h"


auto run_program(std::vector<unsigned int> numbers) {
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


int main(int argc, char **argv) {
    auto input_stream = open_input_file(argc, argv);

    std::vector<unsigned int> numbers;
    std::string num_str;
    while (std::getline(input_stream, num_str, ',')) {
        numbers.push_back(std::stoi(num_str));
    }

    // For part 1, set noun to 12 and verb to 2
    numbers[1] = 12;
    numbers[2] = 2;
    auto part_1_result = run_program(numbers);

    // For part 2, loop over all possible nouns and verbs
    // until the desired output is found
    auto part_2_result = -1;
    unsigned int desired_output = 19690720;
    for (auto noun = 0; noun < 100; ++noun) {
        for (auto verb = 0; verb < 100; ++verb) {
            numbers[1] = noun;
            numbers[2] = verb;
            if (run_program(numbers) == desired_output) {
                part_2_result = 100 * noun + verb;
                break;
            }
        }
        if (part_2_result >= 0) {
            break;
        }
    }

    std::cout << "PART 1" << std::endl;
    std::cout << "Final value at position 0: " << part_1_result << std::endl;
    std::cout << std::endl;
    std::cout << "PART 2" << std::endl;
    std::cout << "100 * noun + verb = " << part_2_result << std::endl;
    return 0;
}
