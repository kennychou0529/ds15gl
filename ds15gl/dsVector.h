#ifndef _DSVECTOR_H
#define _DSVECTOR_H

#include <GL/glut.h>


// 输入 点相对于球心的球坐标 和 球心的直角坐标，输出 点的直角坐标 GLdouble
void dsSphereToOrtho3dv(
    const GLdouble sphere[3],
    const GLdouble center[3],
    GLdouble ortho[3]
);

// 输入 点相对于球心的球坐标 和 点的直角坐标，输出 球心的直角坐标 GLdouble
void dsFindSphereCenter(
    const GLdouble sphere[3],
    const GLdouble ortho[3],
    GLdouble center[3]
);

// 返回向量的长度 GLdouble
GLdouble dsLenth3dv(GLdouble vec[3]);

// 返回向量的长度 GLfloat
GLfloat dsLenth3fv(GLfloat vec[3]);

// 向量的单位化 GLdouble
void dsNormalize3dv(GLdouble vec[3]);

// 向量的单位化 GLfloat
void dsNormalize3fv(GLfloat vec[3]);

// 向量的减法 GLdouble
void dsDiff3dv(
    const GLdouble v1[3],
    const GLdouble v2[3],
    GLdouble ret[3]
);

// 向量的减法 GLfloat
void dsDiff3fv(
    const GLfloat v1[3],
    const GLfloat v2[3],
    GLfloat ret[3]);

// 向量的加法 GLdouble
void dsAdd3dv(
    const GLdouble v1[3],
    const GLdouble v2[3],
    GLdouble ret[3]
);

// 向量的数乘 GLdouble
void dsScalarMulti3dv(
    const GLdouble v[3],
    const GLdouble multiplier,
    GLdouble ret[3]
);

// 向量的点积 GLdouble
GLdouble dsDot3dv(const GLdouble v1[3], const GLdouble v2[3]);

// 向量的叉积 GLdouble
void dsCross3dv(
    const GLdouble v1[3],
    const GLdouble v2[3],
    GLdouble ret[3]
);

// 向量的叉积 GLfloat
void dsCross3fv(
    const GLfloat v1[3],
    const GLfloat v2[3],
    GLfloat ret[3]
);

// 输入 三角形的三个顶点，输出 单位法向量 GLdouble
void dsNormalVectorOfTriangle3dv(
    const GLdouble p1[3],
    const GLdouble p2[3],
    const GLdouble p3[3],
    GLdouble ret[3]
);

// 输入 三角形的三个顶点，输出 单位法向量 GLfloat
void dsNormalVectorOfTriangle3fv(
    const GLfloat p1[3],
    const GLfloat p2[3],
    const GLfloat p3[3],
    GLfloat ret[3]
);

#endif
