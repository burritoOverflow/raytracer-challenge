#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>

namespace utility {
constexpr double EPSILON_ = 0.0001;

inline bool NearEquals(const double a, const double b) {
    return (fabs(a - b) <= EPSILON_ || a == b);
}
}  // namespace utility

#endif  // UTILITY_H
