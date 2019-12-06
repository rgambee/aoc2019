#include "utils.h"


int main(int argc, char **argv) {
    auto input_stream = open_input_file(argc, argv);
    auto numbers = load_intcode_program(input_stream);

    // For part 1, set noun to 12 and verb to 2
    numbers[1] = 12;
    numbers[2] = 2;
    auto part_1_result = run_intcode_program(numbers);

    // For part 2, loop over all possible nouns and verbs
    // until the desired output is found
    auto part_2_result = -1;
    auto desired_output = 19690720;
    for (auto noun = 0; noun < 100; ++noun) {
        for (auto verb = 0; verb < 100; ++verb) {
            numbers[1] = noun;
            numbers[2] = verb;
            if (run_intcode_program(numbers) == desired_output) {
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
