#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>

namespace utility {
static constexpr double EPSILON_ = 0.00001;

inline bool NearEquals(const double a, const double b) {
    return (fabs(a - b) <= EPSILON_ || a == b);
}
}  // namespace utility

#endif  // UTILITY_H
