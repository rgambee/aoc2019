#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << "range-start range-end" << std::endl;
        exit(1);
    }

    int start = std::stoi(argv[1]);
    int end = std::stoi(argv[2]);
    int matches1 = 0, matches2 = 0;
    for (int password = start; password <= end; ++password) {
        bool sorted = true;
        std::array<int, 10> digit_counts{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int prev_digit = -1;
        for (int place = 5; place >= 0; --place) {
            int digit = (password / int(std::round(std::pow(10, place)))) % 10;
            ++digit_counts[digit];
            if (digit < prev_digit) {
                sorted = false;
                break;
            }
            prev_digit = digit;
        }
        int increment1 = 0, increment2 = 0;
        if (sorted) {
            for (auto count: digit_counts) {
                if (count >= 2) {
                    increment1 = 1;
                    if (count == 2) {
                        increment2 = 1;
                        break;
                    }
                }
            }
        }
        matches1 += increment1;
        matches2 += increment2;
    }

    std::cout << "PART 1" << std::endl;
    std::cout << "Number of passwords meeting first set of criteria: ";
    std::cout << matches1 << std::endl;
    std::cout << std::endl;
    std::cout << "PART 2" << std::endl;
    std::cout << "Number of passwords meeting second set of criteria: ";
    std::cout << matches2 << std::endl;
}
