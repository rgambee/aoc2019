#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int required_fuel(int mass) {
    return (mass / 3) - 2;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " input.txt" << std::endl;
        return 1;
    }

    std::ifstream input_stream(argv[1]);
    if (!input_stream) {
        std::cerr << "File not found: " << argv[1] << std::endl;
        return 2;
    }

    std::string line;
    int module_fuel_cost = 0;
    int total_fuel_cost = 0;
    while (std::getline(input_stream, line)) {
        if (line.length() > 0) {
            int mass = std::stoi(line);
            int fuel = required_fuel(mass);
            module_fuel_cost += fuel;
            while (fuel > 0) {
                total_fuel_cost += fuel;
                fuel = required_fuel(fuel);
            }
        }
    }
    input_stream.close();

    std::cout << "PART 1" << std::endl;
    std::cout << "Fuel cost for all modules: " << module_fuel_cost << std::endl;
    std::cout << std::endl;
    std::cout << "PART 2" << std::endl;
    std::cout << "Total fuel cost: " << total_fuel_cost << std::endl;

    return 0;
}
