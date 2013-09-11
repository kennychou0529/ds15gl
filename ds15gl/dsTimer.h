#ifndef _DSTIMER_H
#define _DSTIMER_H

// dsTimer 时间管理器
// 每个时间管理器内置一个计时装置

#include <chrono>
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::steady_clock;

class dsTimer {
public:
    // 令内置计时装置记录下当前时刻
    void recordTime() {
        recorded_time = steady_clock::now();
    }

    // 返回 当前时刻 与 内置计时装置记录下的时刻 的时间差，单位为秒
    double getDurationSecd() {
        return duration_cast<duration<double>>(
                   steady_clock::now() - recorded_time
               ).count();
    }

    // 返回 当前时刻 与 内置计时装置记录下的时刻 的时间差，单位为秒
    float getDurationSecf() {
        return duration_cast<duration<float>>(
                   steady_clock::now() - recorded_time
               ).count();
    }

    // 返回 当前时刻 与 内置计时装置记录下的时刻 的时间差，单位为毫秒(ms)
    long long getDurationMiliseci() {
        return duration_cast<duration<long long, std::milli>>(
                   steady_clock::now() - recorded_time
               ).count();
    }

private:
    steady_clock::time_point recorded_time;
};

#endif