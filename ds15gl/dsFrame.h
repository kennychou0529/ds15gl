#ifndef _DSFRAME_H
#define _DSFRAME_H

#include "dsScene.h"
#include "dsStatusBar.h"
#include "dsMap.h"

class DSFrame {
public:
    DSFrame();
    ~DSFrame() {}
    void initialize();
    void display();
    size_t getFPS() { return fps; }
private:
    void calculateFPS();
    DSScene scene;
    DSStatusBar statusBar;
    //DSMap map;

    // For FPS
    size_t num_frames;
    size_t fps;
    dsTimeManager time_manager;
};

#endif
