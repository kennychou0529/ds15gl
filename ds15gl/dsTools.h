#ifndef _DSTOOLS_H
#define _DSTOOLS_H

#include <GL/glut.h>

#include "dsTextManager.h"

extern int window_width;
extern int window_height;
extern int status_bar_width;

extern dsTextManager dstext;
extern dsTextManager dstext_small;

extern const char* font_file_name;
extern const int font_height;
extern const int font_height_small;

void dsSpecialKeyDown(int key, int x, int y);
void dsSpecialKeyUp(int key, int x, int y);
void dsKeyDown(unsigned char key, int x, int y);
void dsKeyUp(unsigned char key, int x, int y);

//void dsMouseFunc(int key,int x,int y,int);
//void dsPassiveMonitionFunc(int x,int y);

// 显示一个三维坐标系，xyz 轴分别为红绿蓝
void dsShowAxes();

void drawRectange2D(int x,int y,int width,int length);
void fillRectange2D(int x,int y,int width,int length);

#endif
