#ifndef UTIL_RANDOM_HPP
#define UTIL_RANDOM_HPP

#include <cstdlib>
#include <ctime>
#include <span>

#include "util/types.hpp"

/**
 * @param min inclusive min value
 * @param max exclusive max value
 * @return random<T> number in [min, max)
 */
template <typename T>
T random(T min, T max) {
    return min + (T)((f32)rand() / (f32)RAND_MAX * (f32)(max - min));
}

/**
 * Seeds the random function with the time
 * for truly random numbers
 */
inline void seed_time() {
    // srand(time(nullptr));
    srand(std::time(nullptr));
}

/**
 * Chooses a random element from the given ordered container
 * @param vec the "vector"
 * @return reference to the random element
 */
template <typename T>
T &random_choice(const std::span<T> &vec) {
    return vec[random<i32>(0, vec.size())];
}

#endif // UTIL_RANDOM_HPP
