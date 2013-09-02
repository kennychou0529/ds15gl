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
    DSScene scene;
private:
    void calculateFPS();
    
    DSStatusBar statusBar;

    // For FPS
    size_t num_frames;
    size_t fps;
    dsTimer timer;
};

#endif