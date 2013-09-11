#include "dsEye.h"
#include "dsTimer.h"
#include "dsVector2f.h"
#include "dsVector.h"
#include "dsFrame.h"
#include "dsMap.h"

// 移动视线中心点时，每秒移动距离
static const double viewMoveSpeed = 40;

static const GLdouble pi = 3.1415926;

// 旋转眼睛时，每秒转过弧度
static GLdouble rotateSpeed = 2;

// 内置时间管理器
static dsTimer timer;

extern DSFrame frame;

// 眼睛旋转方向
int rdir = STOP;

// 视线中心位置平移方向
int idir = STOP;

// 视线 3D 前进 / 后退因子，只取 UP DOWN STOP
int mdir = STOP;

bool view_auto_moving = false;

static int* playing;

static GLfloat targetX, targetY;

// 眼睛位置，用球坐标 (r, phi, theta) 表示
// 其中，phi 表示与 z 轴的夹角
// theta 表示在 xy 平面的投影的旋转角
GLdouble eye_sphere[3] = { 25.0, pi / 4, -pi / 2 };

static GLdouble eye_sphere_saved[3];

// 视线向上的方向
GLdouble up[3] = { 0.0, 0.0, 1.0 };

// 视线中心点，球坐标的原点
GLdouble center[3] = { 0.0, 0.0, 0.0 };

static GLdouble center_saved[3] = { 0.0, 0.0, 0.0 };

// 相机位置，不直接修改，通过球坐标间接操作
GLdouble eye[3];

GLdouble direction[3];

static inline void saveEyeSphere() {
    eye_sphere_saved[0] = eye_sphere[0];
    eye_sphere_saved[1] = eye_sphere[1];
    eye_sphere_saved[2] = eye_sphere[2];
}

static inline void saveCenter() {
    center_saved[0] = center[0];
    center_saved[1] = center[1];
    center_saved[2] = center[2];
}

static inline void saveDirection() {
    dsDiff3dv(center, eye, direction);
    dsNormalize3dv(direction);
}

// 保存当前视角信息，在 dsTools 中的键盘操作函数中用到
void saveEyeInfo() {
    if (view_auto_moving) {
        return;
    }

    saveEyeSphere();
    saveCenter();
    saveDirection();
    timer.recordTime();
}

void centerMoveTof(GLfloat x, GLfloat y, int* script_playing) {
    saveEyeInfo();
    playing = script_playing;
    if (playing != nullptr) {
        ++(*playing);
    }
    targetX = x;
    targetY = y;
    view_auto_moving = true;
}

void centerMoveToi(size_t x, size_t y, int* script_playing) {
    saveEyeInfo();
    playing = script_playing;
    if (playing != nullptr) {
        ++(*playing);
    }
    frame.scene.map.getCoords(x, y, &targetX, &targetY);
    view_auto_moving = true;
}

static void dsAutoCenterMove() {
    GLdouble duration = timer.getDurationSecd();
    dsVector2f dir(
        GLfloat(targetX - center_saved[0]),
        GLfloat(targetY - center_saved[1])
    );
    GLfloat lenth = dir.getLenth();

    auto endMoving = [&]() {
        center[0] = targetX;
        center[1] = targetY;
        view_auto_moving = false;
        if (playing != nullptr) {
            --(*playing);
        }
        saveEyeInfo();
    };

    if (viewMoveSpeed * 1.0 > lenth) { // 按 viewMoveSpeed 可以在 1 秒内移动完毕
        dir.normalise();
        center[0] = center_saved[0] + dir.x * viewMoveSpeed * duration;
        center[1] = center_saved[1] + dir.y * viewMoveSpeed * duration;
        if (viewMoveSpeed * duration > lenth) {
            endMoving();
        }
    } else { // 按 viewMoveSpeed 不能在 1 秒内移动完毕
        if (duration < 1.0) {
            center[0] = center_saved[0] + (targetX - center_saved[0]) * duration;
            center[1] = center_saved[1] + (targetY - center_saved[1]) * duration;
        } else {
            endMoving();
        }
    }
}

// 按时间设置视线中心点，在 dsSetEye 中调用
static void dsCenterMove() {
    if (view_auto_moving == true) {
        dsAutoCenterMove();
        return;
    }

    dsVector2f dir_up(GLfloat(center[0] - eye[0]), GLfloat(center[1] - eye[1]));
    dsVector2f dir_left = dir_up.turnLeft();

    dsVector2f dir_move(0.0f, 0.0f); // Moving direction
    if (idir & UP) {
        dir_move += dir_up;
    }
    if (idir & DOWN) {
        dir_move -= dir_up;
    }
    if (idir & LEFT) {
        dir_move += dir_left;
    }
    if (idir & RIGHT) {
        dir_move -= dir_left;
    }
    dir_move.normalise();

    int move_factor = 0;
    if (mdir & UP) {
        ++move_factor;
    }
    if (mdir & DOWN) {
        --move_factor;
    }

    double duration = timer.getDurationSecd();
    center[0]
        = center_saved[0]
          + (direction[0] * move_factor + dir_move.x) * viewMoveSpeed * duration;
    center[1]
        = center_saved[1]
          + (direction[1] * move_factor + dir_move.y) * viewMoveSpeed * duration;
    center[2]
        = center_saved[2]
          + direction[2] * move_factor * viewMoveSpeed * duration;
}

// 按时间设置眼睛球坐标，在 dsSetEye 中调用
static void dsEyeRotate() {
    if (rdir == STOP || view_auto_moving) {
        return;
    }

    int factor1 = 0, factor2 = 0;
    if (rdir & UP) {
        ++factor1;
    }
    if (rdir & DOWN) {
        --factor1;
    }
    if (rdir & LEFT) {
        ++factor2;
    }
    if (rdir & RIGHT) {
        --factor2;
    }

    double duration = timer.getDurationSecd();
    eye_sphere[1] = eye_sphere_saved[1] + factor1 * rotateSpeed * duration;
    if (eye_sphere[1] > pi - 0.01) {
        eye_sphere[1] = pi - 0.01;
    }
    if (eye_sphere[1] < 0.01) {
        eye_sphere[1] = 0.01;
    }
    eye_sphere[2] = eye_sphere_saved[2] + factor2 * rotateSpeed * duration;
}

// 设置视角，每次绘图时用到
void dsSetEye() {
    dsEyeRotate();
    dsCenterMove();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 将球坐标转化为直角坐标
    dsSphereToOrtho3dv(eye_sphere, center, eye);

    // 设置摄像头位置
    gluLookAt(
        eye[0], eye[1], eye[2],
        center[0], center[1], center[2],
        up[0], up[1], up[2]
    );

    // 监听者位置
    frame.sounds.setListenerPosition(eye[0], eye[1], eye[2]);
}
