#include "dsVector.h"
#include <cmath>

using std::sqrt;
using std::sin;
using std::cos;

// 输入 点相对于球心的球坐标 和 球心的直角坐标，输出 点的直角坐标 GLdouble
void dsSphereToOrtho3dv(
    const GLdouble sphere[3],
    const GLdouble center[3],
    GLdouble ortho[3]
) {
    ortho[0] = center[0] + sphere[0] * sin(sphere[1]) * cos(sphere[2]);
    ortho[1] = center[1] + sphere[0] * sin(sphere[1]) * sin(sphere[2]);
    ortho[2] = center[2] + sphere[0] * cos(sphere[1]);
}

// 输入 点的直角坐标 和点相对于球心的球坐标，输出 球心的直角坐标 GLdouble
void dsFindSphereCenter(
    const GLdouble sphere[3],
    const GLdouble ortho[3],
    GLdouble center[3]
) {
    center[0] = sphere[0] * sin(sphere[1]) * cos(sphere[2]) - ortho[0];
    center[1] = sphere[0] * sin(sphere[1]) * sin(sphere[2]) - ortho[1];
    center[2] = sphere[0] * cos(sphere[1]) - ortho[2];
}

// 返回向量的长度 GLdouble
GLdouble dsLenth3dv(GLdouble vec[3]) {
    return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

// 返回向量的长度 GLfloat
GLfloat dsLenth3fv(GLfloat vec[3]) {
    return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

// 向量的单位化 GLdouble
void dsNormalize3dv(GLdouble vec[3]) {
    GLdouble len = dsLenth3dv(vec);

    if (len) {
        vec[0] /= len;
        vec[1] /= len;
        vec[2] /= len;
    }
}

// 向量的单位化 GLfloat
void dsNormalize3fv(GLfloat vec[3]) {
    GLfloat len = dsLenth3fv(vec);

    if (len) {
        vec[0] /= len;
        vec[1] /= len;
        vec[2] /= len;
    }
}

// 向量的加法 GLdouble
void dsAdd3dv(
    const GLdouble v1[3],
    const GLdouble v2[3],
    GLdouble ret[3]
) {
    ret[0] = v1[0] + v2[0];
    ret[1] = v1[1] + v2[1];
    ret[2] = v1[2] + v2[2];
}

// 向量的数乘 GLdouble
void dsScalarMulti3dv(
    const GLdouble v[3],
    const GLdouble multiplier,
    GLdouble ret[3]
) {
    ret[0] = v[0] * multiplier;
    ret[1] = v[1] * multiplier;
    ret[2] = v[2] * multiplier;
}

// 向量的点积 GLdouble
GLdouble dsDot3dv(const GLdouble v1[3], const GLdouble v2[3]) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

// 向量的减法 GLdouble
void dsDiff3dv(
    const GLdouble v1[3],
    const GLdouble v2[3],
    GLdouble ret[3]
) {
    ret[0] = v1[0] - v2[0];
    ret[1] = v1[1] - v2[1];
    ret[2] = v1[2] - v2[2];
}

// 向量的减法 GLfloat
void dsDiff3fv(const GLfloat v1[3], const GLfloat v2[3], GLfloat ret[3]) {
    ret[0] = v1[0] - v2[0];
    ret[1] = v1[1] - v2[1];
    ret[2] = v1[2] - v2[2];
}

// 向量的叉积 GLdouble
void dsCross3dv(
    const GLdouble v1[3],
    const GLdouble v2[3],
    GLdouble ret[3]
) {
    ret[0] = v1[1] * v2[2] - v1[2] * v2[1];
    ret[1] = v1[2] * v2[0] - v1[0] * v2[2];
    ret[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

// 向量的叉积 GLfloat
void dsCross3fv(
    const GLfloat v1[3],
    const GLfloat v2[3],
    GLfloat ret[3]
) {
    ret[0] = v1[1] * v2[2] - v1[2] * v2[1];
    ret[1] = v1[2] * v2[0] - v1[0] * v2[2];
    ret[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

// 输入 三角形的三个顶点，输出 单位法向量 GLdouble
void dsNormalVectorOfTriangle3dv(
    const GLdouble p1[3],
    const GLdouble p2[3],
    const GLdouble p3[3],
    GLdouble ret[3]
) {
    GLdouble vec1[3], vec2[3];
    dsDiff3dv(p1, p2, vec1);
    dsDiff3dv(p1, p3, vec2);
    dsCross3dv(vec1, vec2, ret);
    dsNormalize3dv(ret);
}

// 输入 三角形的三个顶点，输出 单位法向量 GLfloat
void dsNormalVectorOfTriangle3fv(
    const GLfloat p1[3],
    const GLfloat p2[3],
    const GLfloat p3[3],
    GLfloat ret[3]
) {
    GLfloat vec1[3], vec2[3];
    dsDiff3fv(p1, p2, vec1);
    dsDiff3fv(p1, p3, vec2);
    dsCross3fv(vec1, vec2, ret);
    dsNormalize3fv(ret);
}
