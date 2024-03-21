#pragma once

#include <iostream>

class Clock {
public:
    void start() {
        log_start_ = std::chrono::steady_clock::now();
    }
    void finish() {
        log_finish_ = std::chrono::steady_clock::now();
    }
    auto result() {
        std::chrono::duration<double, std::ratio<1, 1000000>> duration = log_finish_ - log_start_;
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> log_start_;
    std::chrono::time_point<std::chrono::steady_clock> log_finish_;
};
