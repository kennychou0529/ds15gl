#pragma once
#include "dsScene.h"
#include "dsStatusBar.h"
#include "dsMap.h"

class DSFrame
{
public:
	DSFrame();
	~DSFrame();
public:
	void initialize();
	void display();
	void setSize(int width, int height);
private:
	int window_width;
	int window_height;
private:
	DSScene scene;
	DSStatusBar statusBar;
	DSMap map;
};

