#ifndef UTILITY_H
#define UTILITY_H

#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

// TODO move to a more appropriate subdirectory?
namespace utility {
constexpr double EPSILON_ = 0.0001;

struct ElapsedDuration {
    const long elapsed_minutes_;
    const long elapsed_seconds_;
    const long elapsed_milliseconds_;
};

inline bool NearEquals(const double a, const double b) {
    return (fabs(a - b) <= EPSILON_ || a == b);
}

inline std::string CurrentDateStr() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    const std::string date_fmt_str = "%m-%d-%Y_%H-%M-%S";
    oss << std::put_time(&tm, date_fmt_str.c_str());
    return oss.str();
}

void CreateImageOutdir(std::string_view dirname);

// build a ElapsedDuration struct from a std::chrono duration
ElapsedDuration BuildElapsedDuration(std::chrono::duration<double> total_time);

// output the time taken to invoke the `render_fn` (minutes, seconds, ms)
void OutputMeasuredDuration(const std::function<void()>& render_fn);
}  // namespace utility

#endif  // UTILITY_H
