#ifndef _DSTIMEMANAGER_H
#define _DSTIMEMANAGER_H
#include <chrono>

class dsTimeManager {
public:
    void recordTime() {
        recorded_time = std::chrono::steady_clock::now();
    }

    double getDurationSecd() {
        return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - recorded_time).count();
    }

    float getDurationSecf() {
        return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - recorded_time).count();
    }

    long long getDurationMiliseci() {
        return std::chrono::duration_cast<std::chrono::duration<long long, std::milli>>(std::chrono::steady_clock::now() - recorded_time).count();
    }

private:
    std::chrono::steady_clock::time_point recorded_time;
};

#endif