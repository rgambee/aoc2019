#include <algorithm>
#include <climits>
#include <array>

#include "utils.h"


constexpr auto WIDTH = 25;
constexpr auto HEIGHT = 6;
constexpr auto LAYER_SIZE = WIDTH * HEIGHT;
using layer_type = std::array<int, LAYER_SIZE>;


void render_image(std::vector<layer_type> layers) {
    const auto BLACK_NUM = 0, WHITE_NUM = 1, TRANS_NUM = 2;
    const auto BLACK_CHAR = " ", WHITE_CHAR = "#";
    for (int row = 0; row < HEIGHT; ++row) {
        for (int col = 0; col < WIDTH; ++col) {
            auto i = WIDTH * row + col;
            auto output = BLACK_CHAR;
            for (auto iter = layers.begin(); iter != layers.end(); ++iter) {
                switch ((*iter)[i]) {
                    case BLACK_NUM:
                        output = BLACK_CHAR;
                        break;
                    case WHITE_NUM:
                        output = WHITE_CHAR;
                        break;
                    case TRANS_NUM:
                        continue;
                    default:
                        std::cerr << "Invalid digit " << (*iter)[i] << std::endl;
                        exit(3);
                }
                break;
            }
            std::cout << output;
        }
        std::cout << std::endl;
    }
}


int main(int argc, char **argv) {
    auto input_stream = open_input_file(argc, argv);
    std::vector<layer_type> layers;
    while (input_stream) {
        layer_type new_layer;
        char layer_buffer[LAYER_SIZE];
        input_stream.read(layer_buffer, LAYER_SIZE);
        if (input_stream.gcount() == LAYER_SIZE) {
            for (int i = 0; i < LAYER_SIZE; ++i) {
                new_layer[i] = static_cast<int>(layer_buffer[i] - '0');
            }
            layers.push_back(new_layer);
        }
    }

    auto fewest_zeros = INT_MAX;
    auto layer_with_fewest_zeros = layers.begin();
    for (auto iter = layers.begin(); iter != layers.end(); ++iter) {
        auto num_zeros = std::count(iter->begin(), iter->end(), 0);
        if (num_zeros < fewest_zeros) {
            fewest_zeros = num_zeros;
            layer_with_fewest_zeros = iter;
        }
    }
    auto num_ones = std::count(layer_with_fewest_zeros->begin(),
                               layer_with_fewest_zeros->end(), 1);
    auto num_twos = std::count(layer_with_fewest_zeros->begin(),
                               layer_with_fewest_zeros->end(), 2);

    std::cout << "PART 1" << std::endl;
    std::cout << "Number of 1s times number of 2s " << num_ones * num_twos << std::endl;
    std::cout << std::endl;
    std::cout << "PART 2" << std::endl;
    render_image(layers);

    return 0;
}
