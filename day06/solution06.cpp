#include <memory>
#include <regex>
#include <string>
#include <unordered_map>

#include "utils.h"


class Body {
public:
    Body(const std::string &n): name(n) {}

    void add_satellite(std::shared_ptr<Body> sat) {
        satellites.push_back(sat);
    }

    bool remove_satellite(std::shared_ptr<Body> sat) {
        for (auto iter = satellites.begin(); iter != satellites.end(); ++iter) {
            if (*sat == **iter) {
                satellites.erase(iter);
                return true;
            }
        }
        return false;
    }

    int count_orbits(int num_orbits) {
        auto result = num_orbits;
        for (auto sat: satellites) {
            result += sat->count_orbits(num_orbits + 1);
        }
        return result;
    }

    int get_orbit_depth(std::shared_ptr<Body> sat) {
        if (*sat == *this) {
            return 0;
        }
        for (auto iter = satellites.begin(); iter != satellites.end(); ++ iter) {
            auto child_depth = (*iter)->get_orbit_depth(sat);
            if (child_depth >= 0) {
                return child_depth + 1;
            }
        }
        return -1;
    }

    bool operator==(const Body &other) {
        return name == other.name;
    }

    std::string name;
    std::vector<std::shared_ptr<Body> > satellites;
};


std::shared_ptr<Body> get_body(
    std::unordered_map<std::string, std::shared_ptr<Body> > &bodies,
    const std::string &name) {
    if (bodies.find(name) == bodies.end()) {
        std::cerr << "Could not find body named " << name << std::endl;
        exit(4);
    }
    return bodies[name];
}


int main(int argc, char **argv) {
    auto input_stream = open_input_file(argc, argv);
    std::unordered_map<std::string, std::shared_ptr<Body> > bodies;
    // Example line: ZR5)FZS
    std::regex line_regex("([[:alnum:]]+)\\)([[:alnum:]]+)");
    std::string line;
    while(std::getline(input_stream, line)) {
        std::smatch match;
        if (std::regex_match(line, match, line_regex)) {
            if (match.size() != 3) {
                std::cerr << "Unexpected number of matches for line: " << line << std::endl;
                exit(3);
            }
            auto parent_name = match.str(1);
            auto satellite_name = match.str(2);
            if (bodies.find(parent_name) == bodies.end()) {
                bodies.insert({parent_name, std::make_shared<Body>(parent_name)});
            }
            if (bodies.find(satellite_name) == bodies.end()) {
                bodies.insert({satellite_name, std::make_shared<Body>(satellite_name)});
            }
            bodies[parent_name]->add_satellite(bodies[satellite_name]);
        }
    }
    std::cout << "Number of bodies: " << bodies.size() << std::endl;
    // for (auto pair: bodies) {
    //     std::cout << pair.first << ": " << pair.second->satellites.size() << std::endl;
    // }

    auto COM = get_body(bodies, "COM");
    auto total_orbits = COM->count_orbits(0);

    // Search for outer-most parent of YOU and SAN
    auto common_parent = COM;
    bool keep_looking = true;
    while (keep_looking) {
        keep_looking = false;
        for (auto sat: common_parent->satellites) {
            if (sat->get_orbit_depth(get_body(bodies, "YOU")) >= 0
                && sat->get_orbit_depth(get_body(bodies, "SAN")) >= 0) {
                common_parent = sat;
                keep_looking = true;
                break;
            }
        }
    }
    auto transfers = (common_parent->get_orbit_depth(get_body(bodies, "YOU"))
                      + common_parent->get_orbit_depth(get_body(bodies, "SAN"))
                      // Subtract 2 to exclude transfers to and from
                      // YOU and SAN themselves.
                      - 2);

    std::cout << "PART 1" << std::endl;
    std::cout << "Total number of orbits: " << total_orbits << std::endl;
    std::cout << std::endl;
    std::cout << "PART 2" << std::endl;
    std::cout << "Number of transfers required: " << transfers << std::endl;
    return 0;
}
