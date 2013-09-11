#include "dsFrame.h"
DSFrame::DSFrame() {}

void DSFrame::initialize() {
    num_frames = 0;
    fps = 0;
    timer.recordTime();
    statusBar.init();
    //scene.map.init(15, 10);
    scene.initialize();
    sounds.loadSounds();
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
        fps = static_cast<size_t>(
                  static_cast<GLfloat>(num_frames) / timer.getDurationSecf()
              );
        num_frames = 0;
        timer.recordTime();
    }
}
