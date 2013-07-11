#ifndef _DSFRAME_H
#define _DSFRAME_H

#include "dsScene.h"
#include "dsStatusBar.h"
#include "dsMap.h"

class DSFrame {
public:
    DSFrame() {}
	~DSFrame() {}
	void initialize();
	void display();

private:
	DSScene scene;
	DSStatusBar statusBar;
	DSMap map;
};

#endif
