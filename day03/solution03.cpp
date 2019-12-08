// I misunderstood the problem statment and ended up way overthinking this.
// This solution gives the correct answer, though it's more complicated than
// it needs to be.

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <list>
#include <memory>
#include <sstream>
#include <string>

#include "utils.h"


using coord_type = std::array<int, 2>;


int distance_between(const coord_type &coordA, const coord_type &coordB) {
    return std::abs(coordA[0] - coordB[0]) + std::abs(coordA[1] - coordB[1]);
}


class Node {
public:
    Node() = default;
    Node(coord_type crd): coords(crd) {}

    void add_neighbor(std::shared_ptr<Node> neigh) {
        // Only adds one-way link.
        // Caller should add link in other direction if desired.
        neighbors.push_back(neigh);
    }

    void remove_neighbor(std::shared_ptr<Node> neigh) {
        for (auto iter = neighbors.begin(); iter != neighbors.end(); ++iter) {
            if (*neigh == **iter) {
                neighbors.erase(iter);
                return;
            }
        }
    }

    int get_distance(const Node &other) {
        // Manhattan distance as the crow flies
        // Not distance along the wire
        return distance_between(coords, other.coords);
    }

    int guess_steps() {
        auto fewest_steps = INT_MAX;
        for (auto iter = neighbors.begin(); iter != neighbors.end(); ++iter) {
            if ((*iter)->steps >= 0) {
                fewest_steps = std::min(fewest_steps,
                                        (*iter)->steps + get_distance(**iter));
            }
        }
        if (fewest_steps == INT_MAX) {
            return -1;
        }
        return fewest_steps;
    }

    bool operator==(const Node &other) {
        return coords == other.coords;
    }

    int steps = -1;
    coord_type coords;
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> previous;
    std::vector<std::shared_ptr<Node> > neighbors;
};


std::shared_ptr<Node> construct_wire(const std::string &path) {
    auto origin = std::make_shared<Node>();
    auto current_node = origin;
    std::string segment;
    std::istringstream path_stream{path};
    while (std::getline(path_stream, segment, ',')) {
        auto dir = segment[0];
        auto dist = std::stoi(segment.substr(1));
        auto curr_coord = current_node->coords;
        coord_type new_coord;
        if (dir == 'U') {
            new_coord = coord_type{curr_coord[0], curr_coord[1] + dist};
        } else if (dir == 'D') {
            new_coord = coord_type{curr_coord[0], curr_coord[1] - dist};
        } else if (dir == 'L') {
            new_coord = coord_type{curr_coord[0] - dist, curr_coord[1]};
        } else if (dir == 'R') {
            new_coord = coord_type{curr_coord[0] + dist, curr_coord[1]};
        } else {
            std::cerr << "Unknown direction: " << dir << std::endl;
            exit(3);
        }
        auto new_node = std::make_shared<Node>(new_coord);
        current_node->add_neighbor(new_node);
        current_node->next = new_node;
        new_node->add_neighbor(current_node);
        new_node->previous = current_node;
        current_node = new_node;
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
    std::cerr << "Invalid segment:";
    std::cerr << "(" << coordA[0] << ", " << coordA[1] << ") ";
    std::cerr << "(" << coordB[0] << ", " << coordB[1] << ")" << std::endl;
    exit(4);
}


coord_type find_intersection(const coord_type &wire1_coordA,
                             const coord_type &wire1_coordB,
                             const coord_type &wire2_coordA,
                             const coord_type &wire2_coordB) {
    auto seg1_axis = get_segment_axis(wire1_coordA, wire1_coordB);
    auto seg2_axis = get_segment_axis(wire2_coordA, wire2_coordB);
    if (seg1_axis == seg2_axis) {
        // Segments are parallel
        return coord_type{INT_MIN, INT_MIN};
    }

    auto min1 = std::min(wire1_coordA[seg1_axis], wire1_coordB[seg1_axis]);
    auto max1 = std::max(wire1_coordA[seg1_axis], wire1_coordB[seg1_axis]);
    auto min2 = std::min(wire2_coordA[seg2_axis], wire2_coordB[seg2_axis]);
    auto max2 = std::max(wire2_coordA[seg2_axis], wire2_coordB[seg2_axis]);

    if (   min1 <= wire2_coordA[1-seg2_axis]
        && max1 >= wire2_coordA[1-seg2_axis]
        && min2 <= wire1_coordA[1-seg1_axis]
        && max2 >= wire1_coordA[1-seg1_axis]) {
        if (seg1_axis == 0) {
            // seg1 is horizontal, so it defines the vertical coordinate
            return coord_type{wire2_coordA[1-seg2_axis],
                              wire1_coordA[1-seg1_axis]};
        } else {
            return coord_type{wire1_coordA[1-seg1_axis],
                              wire2_coordA[1-seg2_axis]};
        }
    }
    return coord_type{INT_MIN, INT_MIN};
}


void add_self_intersections(std::shared_ptr<Node> wire) {
    for (auto seg1_start = wire;
         seg1_start != nullptr && seg1_start->next != nullptr;
         seg1_start = seg1_start->next) {
        auto seg1_end = seg1_start->next;
        for (auto seg2_start = seg1_end->next;
             seg2_start != nullptr && seg2_start->next != nullptr;
             seg2_start = seg2_start->next) {
            auto seg2_end = seg2_start->next;
            auto intersection = find_intersection(
                seg1_start->coords, seg1_end->coords,
                seg2_start->coords, seg2_end->coords);
             if (intersection[0] > INT_MIN || intersection[1] > INT_MIN) {
                if (distance_between(intersection, seg1_start->coords) == 0
                    || distance_between(intersection, seg1_end->coords) == 0
                    || distance_between(intersection, seg2_start->coords) == 0
                    || distance_between(intersection, seg2_end->coords) == 0) {
                    // We've already added this intersection
                    continue;
                }
                auto new_node = std::make_shared<Node>(intersection);
                seg1_start->add_neighbor(new_node);
                new_node->add_neighbor(seg1_start);
                seg1_start->remove_neighbor(seg1_end);
                seg1_start->next = new_node;
                new_node->previous = seg1_start;

                seg1_end->add_neighbor(new_node);
                new_node->add_neighbor(seg1_end);
                seg1_end->remove_neighbor(seg1_start);
                new_node->next = seg1_end;
                seg1_end->previous = new_node;

                seg2_start->add_neighbor(new_node);
                new_node->add_neighbor(seg2_start);
                seg2_start->remove_neighbor(seg2_end);

                seg2_end->add_neighbor(new_node);
                new_node->add_neighbor(seg2_end);
                seg2_end->remove_neighbor(seg2_start);
                // seg2_start and seg2_end keep their original
                // next and previous pointers.

                seg1_end = new_node;
            }
        }
    }
}


void compute_steps(std::shared_ptr<Node> origin) {
    origin->steps = 0;
    std::list<std::shared_ptr<Node> > active_nodes(
        origin->neighbors.begin(), origin->neighbors.end());
    while (!active_nodes.empty()) {
        auto fewest_steps = INT_MAX;
        auto fewest_steps_iter = active_nodes.begin();
        for (auto iter = active_nodes.begin(); iter != active_nodes.end(); ++iter) {
            auto steps = (*iter)->guess_steps();
            if (steps < 0) {
                std::cerr << "Unable to guess steps for node at (";
                std::cerr << (*iter)->coords[0] << ", " << (*iter)->coords[1];
                std::cerr << ")" << std::endl;
                exit(6);
            }
            if (steps < fewest_steps) {
                fewest_steps = steps;
                fewest_steps_iter = iter;
            }
        }
        auto current_node = *fewest_steps_iter;
        current_node->steps = fewest_steps;
        active_nodes.insert(active_nodes.end(),
                            current_node->neighbors.begin(),
                            current_node->neighbors.end());
        active_nodes.remove_if([](std::shared_ptr<Node> n) -> bool {
            return n->steps >= 0;
        });
    }
}


int main(int argc, char **argv) {
    auto input_stream = open_input_file(argc, argv);
    std::string line1, line2;
    std::getline(input_stream, line1);
    std::getline(input_stream, line2);
    auto origin1 = construct_wire(line1);
    auto origin2 = construct_wire(line2);

    // Not necessary, apparently
    // add_self_intersections(origin1);
    // add_self_intersections(origin2);

    compute_steps(origin1);
    compute_steps(origin2);

    auto shortest_dist = INT_MAX, fewest_steps = INT_MAX;
    for (auto wire1 = origin1; wire1 != nullptr && wire1->next != nullptr; wire1 = wire1->next) {
        auto wire1_coordA = wire1->coords;
        auto wire1_coordB = wire1->next->coords;
        for (auto wire2 = origin2; wire2 != nullptr && wire2->next != nullptr; wire2 = wire2->next) {
            auto wire2_coordA = wire2->coords;
            auto wire2_coordB = wire2->next->coords;
            auto intersection = find_intersection(
                wire1_coordA, wire1_coordB, wire2_coordA, wire2_coordB);
            if ((intersection[0] != 0 || intersection[1] != 0)
                && (intersection[0] > INT_MIN || intersection[1] > INT_MIN)) {
                auto new_dist = distance_between(intersection, {0, 0});
                shortest_dist = std::min(shortest_dist, new_dist);

                auto steps1 = std::min(
                                wire1->steps + distance_between(
                                        intersection, wire1_coordA),
                                wire1->next->steps + distance_between(
                                        intersection, wire1_coordB));
                auto steps2 = std::min(
                                wire2->steps + distance_between(
                                        intersection, wire2_coordA),
                                wire2->next->steps + distance_between(
                                        intersection, wire2_coordB));
                fewest_steps = std::min(fewest_steps, steps1 + steps2);
            }
        }
    }

    std::cout << "PART 1" << std::endl;
    std::cout << "Distance to nearest intersection: " << shortest_dist << std::endl;
    std::cout << std::endl;
    std::cout << "PART 2" << std::endl;
    std::cout << "Fewest steps to intersection: " << fewest_steps << std::endl;
    return 0;
}
