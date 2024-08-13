#ifndef HELPER_H
#define HELPER_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 2 * acos(0.0);



inline double RandomDouble() {
    return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max) {
    return min + (max-min)*RandomDouble();
}



inline uint32_t XOrShift32(uint32_t *state)
{
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}



template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
    return clamp(v, lo, hi, std::less{});
}

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif