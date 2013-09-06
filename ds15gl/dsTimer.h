#ifndef _DSTIMER_H
#define _DSTIMER_H

// dsTimer 时间管理器
// 每个时间管理器内置一个计时装置

#include <chrono>

class dsTimer {
public:
    // 令内置计时装置记录下当前时刻
    void recordTime() {
        recorded_time = std::chrono::steady_clock::now();
    }

    // 返回 当前时刻 与 内置计时装置记录下的时刻 的时间差，单位为秒
    double getDurationSecd() {
        return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - recorded_time).count();
    }

    // 返回 当前时刻 与 内置计时装置记录下的时刻 的时间差，单位为秒
    float getDurationSecf() {
        return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - recorded_time).count();
    }

    // 返回 当前时刻 与 内置计时装置记录下的时刻 的时间差，单位为毫秒(ms)
    long long getDurationMiliseci() {
        return std::chrono::duration_cast<std::chrono::duration<long long, std::milli>>(std::chrono::steady_clock::now() - recorded_time).count();
    }

private:
    std::chrono::steady_clock::time_point recorded_time;
};

#endif