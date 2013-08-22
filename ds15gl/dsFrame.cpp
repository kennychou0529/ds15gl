#include "dsFrame.h"
DSFrame::DSFrame() {}

void DSFrame::initialize() {
    num_frames = 0;
    fps = 0;
    time_manager.recordTime();

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
    if (time_manager.getDurationSecf() > 1.0f) {
        fps = (unsigned int)((float)num_frames / time_manager.getDurationSecf());
        num_frames = 0;
        time_manager.recordTime();
    }
}
