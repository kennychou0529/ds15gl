#ifndef _DSTIMER_H
#define _DSTIMER_H

// dsTimer ʱ�������
// ÿ��ʱ�����������һ����ʱװ��

#include <chrono>

class dsTimer {
public:
    // �����ü�ʱװ�ü�¼�µ�ǰʱ��
    void recordTime() {
        recorded_time = std::chrono::steady_clock::now();
    }

    // ���� ��ǰʱ�� �� ���ü�ʱװ�ü�¼�µ�ʱ�� ��ʱ����λΪ��
    double getDurationSecd() {
        return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - recorded_time).count();
    }

    // ���� ��ǰʱ�� �� ���ü�ʱװ�ü�¼�µ�ʱ�� ��ʱ����λΪ��
    float getDurationSecf() {
        return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - recorded_time).count();
    }

    // ���� ��ǰʱ�� �� ���ü�ʱװ�ü�¼�µ�ʱ�� ��ʱ����λΪ����(ms)
    long long getDurationMiliseci() {
        return std::chrono::duration_cast<std::chrono::duration<long long, std::milli>>(std::chrono::steady_clock::now() - recorded_time).count();
    }

private:
    std::chrono::steady_clock::time_point recorded_time;
};

#endif