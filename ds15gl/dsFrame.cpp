#include "dsFrame.h"
DSFrame::DSFrame() {}

void DSFrame::initialize() {
    num_frames = 0;
    fps = 0;
    timer.recordTime();
    statusBar.init();
    scene.initialize();
}

void DSFrame::display() {
    // 计算 FPS，每秒统计一次
    calculateFPS();
    scene.show();
    statusBar.show();
}

void DSFrame::calculateFPS() {
    ++num_frames;
    if (timer.getDurationSecf() > 1.0f) {
        fps = (unsigned int)((float)num_frames / timer.getDurationSecf());
        num_frames = 0;
        timer.recordTime();
    }
}
