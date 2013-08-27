#ifndef _DSEYE_H
#define _DSEYE_H

#include <GL/glut.h>

// 眼睛位置，用球坐标 (r, phi, theta) 表示
// 其中，phi 表示与 z 轴的夹角
// theta 表示在 xy 平面的投影的旋转角 
extern GLdouble eye_sphere[3];

// 视线向上的方向
extern GLdouble up[3];

// 视线中心点，球坐标的原点
extern GLdouble center[3];

// 相机位置，不直接修改，通过球坐标间接操作
extern GLdouble eye[3];

// 四种方向
enum Direction {
    STOP = 0,
    UP = 1,
    DOWN = 1 << 1,
    LEFT = 1 << 2,
    RIGHT = 1 << 3,
};

// 眼睛旋转方向
extern int rdir;

// 视线中心位置 2D 平移方向
extern int idir;

// 视线 3D 前进 / 后退因子，只取 UP DOWN STOP
extern int mdir;

// 保存当前视角信息，在 dsTools 中的键盘操作函数中用到
void saveEyeInfo();

// 设置视角，每次绘图时用到
void dsSetEye();

#endif