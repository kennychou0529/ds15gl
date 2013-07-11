#ifndef _DSSTATUSBAR_H
#define _DSSTATUSBAR_H

#include <GL/glut.h>

class DSStatusBar {
public:
	DSStatusBar();
	~DSStatusBar();
public:
	void show();
	void setSize(int width, int height);
};

#endif
