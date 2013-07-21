#ifndef _DSVECTOR_H
#define _DSVECTOR_H

#include <GL/glut.h>
#include <cmath>

using std::sqrt;
using std::sin;
using std::cos;

// 输入 点相对于球心的球坐标 和 球心的直角坐标，输出 点的直角坐标
void dsSphereToOrtho3dv(const GLdouble sphere[3], const GLdouble center[3], GLdouble ortho[3]);

// 输入 点相对于球心的球坐标 和 点的直角坐标，输出 球心的直角坐标
void dsFindSphereCenter(const GLdouble sphere[3], const GLdouble ortho[3], GLdouble center[3]);

// 返回向量的长度
GLdouble dsLenth3dv(GLdouble vec[3]);

// 向量的单位化
void dsNormalize3dv(GLdouble vec[3]);

// 向量的减法
void dsDiff3dv(const GLdouble v1[3], const GLdouble v2[3], GLdouble ret[3]);

// 向量的加法
void dsAdd3dv(const GLdouble v1[3], const GLdouble v2[3], GLdouble ret[3]);

// 向量的数乘
void dsScalarMulti3dv(const GLdouble v[3], const GLdouble multiplier, GLdouble ret[3]);

// 向量的点积
GLdouble dsDot3dv(const GLdouble v1[3], const GLdouble v2[3]);

// 向量的叉积
void dsCross3dv(const GLdouble v1[3], const GLdouble v2[3], GLdouble ret[3]);

// 输入 三角形的三个顶点，输出 单位法向量
void dsNormalVectorOfTriangle3dv(const GLdouble p1[3], const GLdouble p2[3], const GLdouble p3[3], GLdouble ret[3]);


#endif
