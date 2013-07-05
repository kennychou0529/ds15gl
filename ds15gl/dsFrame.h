#pragma once
#include "dsScene.h"
#include "dsStatusBar.h"
#include "dsMap.h"

class dsFrame
{
public:
	dsFrame(void);
	~dsFrame(void);
public:
	void initializ(void);
	void display(void);
	void setSize(int width,int height);
private:
	int window_width;
	int window_height;
private:
	dsScene scene;
	dsStatusBar statusBar;
	dsMap map;
};

