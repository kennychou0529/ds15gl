#pragma once
#include <gl/glut.h>

class DSStatusBar
{
public:
	DSStatusBar(void);
	~DSStatusBar(void);
public:
	void show();
	void setSize(int width,int height);
private:
	//窗口大小
	int window_width;
	int window_height;
	//状态栏宽度
	int width;
};

