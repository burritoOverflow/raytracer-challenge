#include "utility.h"
#include <chrono>
#include <filesystem>
#include <string_view>

void utility::CreateImageOutdir(const std::string_view dirname) {
    if (!std::filesystem::exists(dirname)) {
        std::filesystem::create_directory(dirname);
    }
}

utility::ElapsedDuration utility::BuildElapsedDuration(
    const std::chrono::duration<double> total_time) {
    const auto elapsed_min = std::chrono::duration_cast<std::chrono::minutes>(total_time).count();
    const auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count();
    const auto elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(total_time).count();

    return {elapsed_min, elapsed_s, elapsed_ms};
}

void utility::OutputMeasuredDuration(const std::function<void()>& render_fn) {
    const auto start_time = std::chrono::steady_clock::now();
    render_fn();
    const auto duration =
        utility::BuildElapsedDuration(std::chrono::steady_clock::now() - start_time);

    long output_milliseconds;
    long output_seconds = duration.elapsed_seconds_;

    // remaining seconds w/o minutes
    if (duration.elapsed_minutes_ > 0) {
        output_seconds -= duration.elapsed_minutes_ * 60;
    }

    // as above, but for milliseconds
    if (duration.elapsed_seconds_ > 1l) {
        output_milliseconds = duration.elapsed_milliseconds_ %
                              (duration.elapsed_seconds_ * static_cast<long>(pow(10, 3)));
    } else {
        output_milliseconds = duration.elapsed_milliseconds_;
    }

    std::clog << "\n\rTotal Duration: " << duration.elapsed_minutes_ << "m " << output_seconds
              << "s " << output_milliseconds << " ms "
              << "\n";
}
