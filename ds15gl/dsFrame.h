#pragma once
#include "dsScene.h"
#include "dsStatusBar.h"
#include "dsMap.h"

class DSFrame
{
public:
	DSFrame(void);
	~DSFrame(void);
public:
	void initialize(void);
	void display(void);
	void setSize(int width,int height);
private:
	int window_width;
	int window_height;
private:
	DSScene scene;
	DSStatusBar statusBar;
	DSMap map;
};

