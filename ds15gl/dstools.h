#ifndef _DSTOOLS_H
#define _DSTOOLS_H

#include "dsvector.h"
#include "dstexture.h"
#include "dslight.h"
#include "vector2f.h"

extern GLdouble eye_sphere[3];
extern GLdouble up[3];
extern GLdouble center[3];
extern GLdouble eye[3];

void dsSet();
void dsSpecialKeys(int key, int x, int y);
void keyFunc(unsigned char key,int x,int y);
//void dsMouseFunc(int key,int x,int y,int);
//void dsPassiveMonitionFunc(int x,int y);

// 显示一个三维坐标系，xyz 轴分别为红绿蓝
void dsShowAxes(GLdouble lenth = 100.0);

#endif