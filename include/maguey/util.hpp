//
// Created by rdelfin on 7/8/17.
//

#ifndef BOIDS_UTIL_HPP
#define BOIDS_UTIL_HPP

#include <string>
#include <vector>

namespace maguey {

/**
 * Splits a string into a vector of strings by a delimiter
 *
 * @param string    The string to split
 * @param delimiter The character to split the string by.
 */
std::vector<std::string> split(const std::string& string, char delimiter);

} // namespace maguey

#endif //BOIDS_UTIL_HPP
