#ifndef _DSVECTOR_H
#define _DSVECTOR_H

#include <GL/glut.h>
#include <cmath>

using std::sqrt;
using std::sin;
using std::cos;

void dsSphereToOrtho3dv(const GLdouble sphere[3], const GLdouble center[3],GLdouble ortho[3]);
GLdouble dsLenth3dv(GLdouble vec[3]);
void dsNormalize3dv(GLdouble vec[3]);
void dsDiff3dv(const GLdouble v1[3], const GLdouble v2[3], GLdouble ret[3]);
void dsCross3dv(const GLdouble v1[3], const GLdouble v2[3], GLdouble ret[3]);
void dNormalVectorOfTriangle3dv(const GLdouble p1[3], const GLdouble p2[3], const GLdouble p3[3], GLdouble ret[3]);


#endif