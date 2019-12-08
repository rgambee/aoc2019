#include <algorithm>
#include <array>
#include <climits>
#include <sstream>

#include "utils.h"


using phase_settings_type = std::array<int, 5>;


int simulate_phase_settings(std::vector<int> program,
                            const phase_settings_type &phase_settings) {
    int amp_output = 0;
    for (auto setting: phase_settings) {
        std::stringstream program_input;
        std::stringstream program_output;
        program_input << setting << " " << amp_output;
        run_intcode_program(program, program_input, program_output);
        program_output >> amp_output;
    }
    return amp_output;
}


int main(int argc, char **argv) {
    auto input_stream = open_input_file(argc, argv);
    auto program = load_intcode_program(input_stream);

    std::array<int, 5> phase_settings{0, 1, 2, 3, 4};
    auto max_signal = INT_MIN;
    do {
        auto signal = simulate_phase_settings(program, phase_settings);
        if (signal > max_signal) {
            max_signal = signal;
        }
    } while (std::next_permutation(phase_settings.begin(), phase_settings.end()));


    std::cout << "PART 1" << std::endl;
    std::cout << "Max thruster signal: " << max_signal << std::endl;
    std::cout << std::endl;
    std::cout << "PART 2" << std::endl;
    return 0;
}
