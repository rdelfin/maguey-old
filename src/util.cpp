//
// Created by rdelfin on 7/8/17.
//

#define GLM_FORCE_RADIANS

#include <maguey/util.hpp>

#include <sstream>
#include <string>
#include <vector>

namespace maguey {

std::vector<std::string> split(const std::string& string, char delimiter) {
    std::vector<std::string> elems;
    std::stringstream ss(string);

    std::string elem;
    while(std::getline(ss, elem, delimiter)) {
        elems.push_back(elem);
    }

    return elems;
}

} // namespace maguey