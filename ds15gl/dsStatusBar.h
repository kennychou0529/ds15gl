#pragma once
#include <gl/glut.h>

class dsStatusBar
{
public:
	dsStatusBar(void);
	~dsStatusBar(void);
public:
	void show();
	void setSize(int width,int height);
private:
	//���ڴ�С
	int window_width;
	int window_height;
	//״̬������
	int width;
};
