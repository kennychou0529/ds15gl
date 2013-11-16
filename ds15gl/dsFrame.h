#ifndef _DSFRAME_H
#define _DSFRAME_H

#include "dsScene.h"
#include "dsStatusBar.h"
#include "dsMap.h"
#include "dsSoundManager.h"
#include "dsActorManager.h"

class DSFrame {
public:
    DSFrame();
    ~DSFrame() {}
    void initialize(const std::string& rep_file_name);
	void initialize2(const std::string& file_name);
    void display(bool selectMode = false);
    size_t getFPS() { return fps; }
    
public:
	DSScene scene;
	DSSoundManager sounds;
	DSActorManager actors;
	
private:
    void calculateFPS();
    
    DSStatusBar statusBar;

    // For FPS
    size_t num_frames;
    size_t fps;
    dsTimer timer;
};

#endif