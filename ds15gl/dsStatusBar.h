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
private:
	//���ڴ�С
	int window_width;
	int window_height;
	//״̬�����
	int width;
};

#endif
