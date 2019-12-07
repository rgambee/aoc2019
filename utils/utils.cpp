#include <cmath>
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
    // Could simply cast to an Opcode,
    // but that wouldn't reject unknown inputs.
    switch(integer % 100) {
        case 1:
            return Opcode::ADD;
        case 2:
            return Opcode::MULTIPLY;
        case 3:
            return Opcode::INPUT;
        case 4:
            return Opcode::OUTPUT;
        case 5:
            return Opcode::JUMP_TRUE;
        case 6:
            return Opcode::JUMP_FALSE;
        case 7:
            return Opcode::LESS_THAN;
        case 8:
            return Opcode::EQUALS;
        case 99:
            return Opcode::END;
        default:
            std::cerr << "Unknown opcode: " << integer << std::endl;
            exit(3);
    }
}


std::vector<Mode> int_to_modes(int integer, int num_operands) {
    std::vector<Mode> result(num_operands, Mode::POSITIONAL);
    integer /= 100;     // Remove opcode (trailing two digits)
    for (auto place = 0; place < num_operands; place++) {
        int digit = (integer / int(std::round(std::pow(10, place)))) % 10;
        switch(digit) {
            case 0:
                result[place] = Mode::POSITIONAL;
                break;
            case 1:
                result[place] = Mode::IMMEDIATE;
                break;
            default:
                std::cerr << "Unknown mode: " << digit;
                exit(3);
        }
    }
    return result;
}


std::vector<int> load_intcode_program(std::istream &input_stream) {
    std::vector<int> numbers;
    std::string num_str;
    while (std::getline(input_stream, num_str, ',')) {
        numbers.push_back(std::stoi(num_str));
    }
    return numbers;
}


void check_index(int index, const std::vector<int> &numbers) {
    if (index < 0 || int(numbers.size()) <= index) {
        std::cerr << "Index out of range: " << index << std::endl;
        exit(4);
    }
}


int run_intcode_program(std::vector<int> numbers,
                        std::istream &input,
                        std::ostream &output) {
    for (std::vector<int>::size_type i = 0; i < numbers.size();) {
        auto opcode = int_to_opcode(numbers[i]);
        switch (opcode) {
            case Opcode::END:
                i = numbers.size();
                break;
            case Opcode::INPUT:
            case Opcode::OUTPUT: {
                int num_operands = 1;
                auto modes = int_to_modes(numbers[i], num_operands);
                if (opcode == Opcode::INPUT && modes[0] != Mode::POSITIONAL) {
                    std::cerr << "Opcode " << int(opcode);
                    std::cerr << " expects positional operand mode" << std::endl;
                    exit(3);
                }
                auto parameter = numbers[i+1];
                if (modes[0] == Mode::POSITIONAL) {
                    check_index(parameter, numbers);
                }
                switch (opcode) {
                    case Opcode::INPUT:
                        input >> numbers[parameter];
                        break;
                    case Opcode::OUTPUT:
                        switch (modes[0]) {
                            case Mode::POSITIONAL:
                                output << numbers[parameter];
                                break;
                            case Mode::IMMEDIATE:
                                output << parameter;
                                break;
                            default:
                                std::cerr << "Unexpected mode: " << int(modes[0]) << std::endl;
                                exit(3);
                        }
                        break;
                    default:
                        std::cerr << "Unexpected opcode: " << int(opcode) << std::endl;
                        exit(3);
                }
                i += num_operands + 1;
                break;
            }
            case Opcode::JUMP_TRUE:
            case Opcode::JUMP_FALSE: {
                int num_operands = 2;
                auto modes = int_to_modes(numbers[i], num_operands);
                int condition = -1, destination = -1;
                switch (modes[0]) {
                    case Mode::POSITIONAL:
                        check_index(numbers[i+1], numbers);
                        condition = numbers[numbers[i+1]];
                        break;
                    case Mode::IMMEDIATE:
                        condition = numbers[i+1];
                        break;
                    default:
                        std::cerr << "Unexpected mode: " << int(modes[0]) << std::endl;
                        exit(3);
                }
                switch (modes[1]) {
                    case Mode::POSITIONAL:
                        check_index(numbers[i+2], numbers);
                        destination = numbers[numbers[i+2]];
                        break;
                    case Mode::IMMEDIATE:
                        destination = numbers[i+2];
                        break;
                    default:
                        std::cerr << "Unexpected mode: " << int(modes[1]) << std::endl;
                        exit(3);
                }
                switch (opcode) {
                    case Opcode::JUMP_TRUE:
                        i = condition ? destination : i + num_operands + 1;
                        break;
                    case Opcode::JUMP_FALSE:
                        i = !condition ? destination : i + num_operands + 1;
                        break;
                    default:
                        std::cerr << "Unexpected opcode: " << int(opcode) << std::endl;
                        exit(3);
                }
                break;
            }
            case Opcode::ADD:
            case Opcode::MULTIPLY:
            case Opcode::LESS_THAN:
            case Opcode::EQUALS: {
                int num_operands = 3;
                auto modes = int_to_modes(numbers[i], num_operands);
                int input_a = -1, input_b = -1;
                switch (modes[0]) {
                    case Mode::POSITIONAL:
                        check_index(numbers[i+1], numbers);
                        input_a = numbers[numbers[i+1]];
                        break;
                    case Mode::IMMEDIATE:
                        input_a = numbers[i+1];
                        break;
                    default:
                        std::cerr << "Unexpected mode: " << int(modes[0]) << std::endl;
                        exit(3);
                }
                switch (modes[1]) {
                    case Mode::POSITIONAL:
                        check_index(numbers[i+2], numbers);
                        input_b = numbers[numbers[i+2]];
                        break;
                    case Mode::IMMEDIATE:
                        input_b = numbers[i+2];
                        break;
                    default:
                        std::cerr << "Unexpected mode: " << int(modes[1]) << std::endl;
                        exit(3);
                }
                if (modes[2] != Mode::POSITIONAL) {
                    std::cerr << "Opcode " << int(opcode);
                    std::cerr << " expects positional mode for final operand" << std::endl;
                    exit(3);
                }
                auto output_index = numbers[i+3];
                check_index(output_index, numbers);
                int result = -1;
                switch (opcode) {
                    case Opcode::ADD:
                        result = input_a + input_b;
                        break;
                    case Opcode::MULTIPLY:
                        result = input_a * input_b;
                        break;
                    case Opcode::LESS_THAN:
                        result = input_a < input_b ? 1 : 0;
                        break;
                    case Opcode::EQUALS:
                        result = input_a == input_b ? 1 : 0;
                        break;
                    default:
                        std::cerr << "Unexpected opcode: " << int(opcode) << std::endl;
                        exit(3);
                }
                numbers[output_index] = result;
                i += num_operands + 1;
                break;
            }
            default:
                std::cerr << "Unexpected opcode: " << int(opcode) << std::endl;
                exit(3);
        }
    }
    return numbers[0];
}
