#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <memory>
#include <sstream>
#include <string>

#include "utils.h"


using coord_type = std::array<int, 2>;


class Node {
public:
    Node(int ord): order(ord) {}
    Node(int ord, coord_type crd): order(ord), coords(crd) {}

    void add_neighbor(std::shared_ptr<Node> neigh) {
        // Only adds one-way link.
        // Caller should add link in other direction if desired.
        neighbors.push_back(neigh);
    }

    std::shared_ptr<Node> get_next_node() {
        for (auto neigh: neighbors) {
            if (neigh->order == order + 1) {
                return neigh;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Node> get_prev_node() {
        for (auto neigh: neighbors) {
            if (neigh->order == order - 1) {
                return neigh;
            }
        }
        return nullptr;
    }

    int order = -1;
    coord_type coords;
    std::vector<std::shared_ptr<Node> > neighbors;
};


std::shared_ptr<Node> construct_wire(const std::string &path) {
    auto origin = std::make_shared<Node>(0);
    auto current_node = origin;
    std::string segment;
    std::istringstream path_stream{path};
    while (std::getline(path_stream, segment, ',')) {
        auto dir = segment[0];
        auto dist = std::stoi(segment.substr(1));
        auto prev_coord = wire_coords.back();
        coord_type new_coord;
        if (dir == 'U') {
            new_coord = coord_type{prev_coord[0], prev_coord[1] + dist};
        } else if (dir == 'D') {
            new_coord = coord_type{prev_coord[0], prev_coord[1] - dist};
        } else if (dir == 'L') {
            new_coord = coord_type{prev_coord[0] - dist, prev_coord[1]};
        } else if (dir == 'R') {
            new_coord = coord_type{prev_coord[0] + dist, prev_coord[1]};
        } else {
            std::cerr << "Unknown direction: " << dir << std::endl;
            exit(3);
        }
        auto new_node = std::make_shared<Node>(current_node->order + 1, new_coord);
        current_node->add_neighbor(new_node);
        new_node->add_neighbor(current_node);
    }
    return origin;
}


int get_segment_axis(const coord_type &coordA,
                     const coord_type &coordB) {
    if (coordA[0] == coordB[0] && coordA[1] != coordB[1]) {
        // Vertical
        return 1;
    } else if (coordA[0] != coordB[0] && coordA[1] == coordB[1]) {
        // Horizontal
        return 0;
    }
    std::cerr << "Invalid segment" << std::endl;
    exit(4);
}


coord_type find_intersection(const coord_type &wire1_coordA,
                             const coord_type &wire1_coordB,
                             const coord_type &wire2_coordA,
                             const coord_type &wire2_coordB) {
    auto segment1_axis = get_segment_axis(wire1_coordA, wire1_coordB);
    auto segment2_axis = get_segment_axis(wire2_coordA, wire2_coordB);
    if (segment1_axis == segment2_axis) {
        // Segments are parallel
        return coord_type{INT_MIN, INT_MIN};
    }

    auto min1 = std::min(wire1_coordA[segment1_axis], wire1_coordB[segment1_axis]);
    auto max1 = std::max(wire1_coordA[segment1_axis], wire1_coordB[segment1_axis]);
    auto min2 = std::min(wire2_coordA[segment2_axis], wire2_coordB[segment2_axis]);
    auto max2 = std::max(wire2_coordA[segment2_axis], wire2_coordB[segment2_axis]);

    if (   min1 <= wire2_coordA[1-segment2_axis]
        && max1 >= wire2_coordA[1-segment2_axis]
        && min2 <= wire1_coordA[1-segment1_axis]
        && max2 >= wire1_coordA[1-segment1_axis]) {
        return coord_type{wire1_coordA[1-segment1_axis],
                          wire2_coordA[1-segment2_axis]};
    }
    return coord_type{INT_MIN, INT_MIN};
}


int main(int argc, char **argv) {
    auto input_stream = open_input_file(argc, argv);
    std::string line1, line2;
    std::getline(input_stream, line1);
    std::getline(input_stream, line2);
    auto wire1 = construct_wire(line1);
    auto wire2 = construct_wire(line2);

    // Find self-intersections and add to list of nodes

    auto shortest_dist = INT_MAX, fewest_steps = INT_MAX;
    auto steps1 = 0;
    for (auto iter1 = wire_coords1.cbegin(); iter1 < wire_coords1.cend() - 1; ++iter1) {
        auto wire1_coordA = *iter1;
        auto wire1_coordB = *(iter1+1);
        auto steps2 = 0;
        for (auto iter2 = wire_coords2.cbegin(); iter2 < wire_coords2.cend() - 1; ++iter2) {
            std::cout << "steps1 = " << steps1 << ", steps2 = " << steps2 << std::endl;
            auto wire2_coordA = *iter2;
            auto wire2_coordB = *(iter2+1);
            auto intersection = find_intersection(
                wire1_coordA, wire1_coordB, wire2_coordA, wire2_coordB);
            if (intersection[0] > INT_MIN || intersection[1] > INT_MIN) {
                auto new_dist = std::abs(intersection[0]) + std::abs(intersection[1]);
                if (new_dist > 0) {
                    std::cout << "Intersection at (" << intersection[0];
                    std::cout << ", " << intersection[1] << ")" << std::endl;
                    std::cout << "Distance: " << new_dist << std::endl;
                    if (new_dist < shortest_dist) {
                        shortest_dist = new_dist;
                    }
                    std::cout << std::abs(wire1_coordA[0] - intersection[0]) << std::endl;
                    std::cout << std::abs(wire1_coordA[1] - intersection[0]) << std::endl;
                    std::cout << std::abs(wire2_coordA[0] - intersection[1]) << std::endl;
                    std::cout << std::abs(wire2_coordA[1] - intersection[1]) << std::endl;
                    auto new_steps = (steps1 + steps2
                                      + std::abs(wire1_coordA[0] - intersection[0])
                                      + std::abs(wire1_coordA[1] - intersection[0])
                                      + std::abs(wire2_coordA[0] - intersection[1])
                                      + std::abs(wire2_coordA[1] - intersection[1]));
                    std::cout << "Steps: " << new_steps << std::endl;
                    if (new_steps < fewest_steps) {
                        fewest_steps = new_steps;
                    }
                }
            }
            steps2 += (std::abs(wire2_coordA[0] - wire2_coordB[0])
                       + std::abs(wire2_coordA[1] - wire2_coordB[1]));
        }
        steps1 += (std::abs(wire1_coordA[0] - wire1_coordB[0])
                   + std::abs(wire1_coordA[1] - wire1_coordB[1]));
    }

    std::cout << "PART 1" << std::endl;
    std::cout << "Distance to nearest intersection: " << shortest_dist << std::endl;
    std::cout << std::endl;
    std::cout << "PART 2" << std::endl;
    std::cout << "Fewest steps to intersection: " << fewest_steps << std::endl;
    return 0;
}
