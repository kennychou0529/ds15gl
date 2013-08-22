#include "dsTools.h"
#include <mutex>
#include <thread>
#include <iostream>
#include <iomanip>
#include "dsLight.h"
#include "dsVector2f.h"
#include "dsVector.h"

#include "dsSoundManager.h"

static const double viewMoveSpeed = 1.0;
static const GLdouble pi = 3.1415926;

// 眼睛位置，用球坐标 (r, phi, theta) 表示
// 其中，phi 表示与 z 轴的夹角
// theta 表示在 xy 平面的投影的旋转角
GLdouble eye_sphere[3] = {10.0, pi / 4, - pi / 2};

// 全局使用
int window_width = 1280;
int window_height = 720;
int status_bar_width = 200;

//dsTimeManager time_manager;
//dsTimeManager time_manager2;
//unsigned int frame_count;
//unsigned int fps;

// 文字管理器
dsTextManager dstext;
dsTextManager dstext_small;


//声音管理
DSSoundManager* soundManager = DSSoundManager::getSoundManager();


#ifdef WIN32
const char* font_file_name = "C:/Windows/Fonts/msyhbd.ttc";
// const char* font_file_name = "C:/Windows/Fonts/wqy-zenhei.ttc";
// const char* font_file_name = "data/xhei_bold.ttf";
#elif defined __unix__
const char* font_file_name = "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc";
#endif

const int font_height = 32;
const int font_height_small = 13;

// 勿用
// GLdouble center_sphere[3] = {20.0, 3 * pi / 4, - pi / 2};

GLdouble up[3] = {0.0, 0.0, 1.0};

// 视线中心点，球坐标的原点
GLdouble center[3] = {0.0, 0.0, 0.0};

// 相机位置，不直接修改，通过球坐标间接操作
GLdouble eye[3];

GLdouble axeLength = eye_sphere[0] * 0.5;

void dsSetEye() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 将球坐标转化为直角坐标
    dsSphereToOrtho3dv(eye_sphere, center, eye);

    // 设置摄像头位置
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);


	// 监听者位置
	soundManager->setListenerPosition(eye[0], eye[1], eye[2]);
}

void dsSpecialKeys(int key, int x, int y) {
    static GLdouble rotateSpeed = 0.1;
    static GLdouble scaleSpeed = 0.5;
    GLdouble direction[3];

    switch (key) {
    case GLUT_KEY_UP:
        if (eye_sphere[1] < 3.1415926 - rotateSpeed) {
            eye_sphere[1] += rotateSpeed;
        }

        break;

    case GLUT_KEY_DOWN:
        if (eye_sphere[1] > rotateSpeed) {
            eye_sphere[1] -= rotateSpeed;
        }

        break;

    case GLUT_KEY_LEFT:
        eye_sphere[2] += rotateSpeed;
        break;

    case GLUT_KEY_RIGHT:
        eye_sphere[2] -= rotateSpeed;
        break;

    case GLUT_KEY_F1:
        dsDiff3dv(center, eye, direction);
        dsNormalize3dv(direction);
        center[0] += direction[0] * viewMoveSpeed;
        center[1] += direction[1] * viewMoveSpeed;
        center[2] += direction[2] * viewMoveSpeed;
        break;

    case GLUT_KEY_F2:
        dsDiff3dv(center, eye, direction);
        dsNormalize3dv(direction);
        center[0] -= direction[0] * viewMoveSpeed;
        center[1] -= direction[1] * viewMoveSpeed;
        center[2] -= direction[2] * viewMoveSpeed;
        break;
    }

    axeLength = eye_sphere[0] * 0.5;
}

void dsKeys(unsigned char key, int x, int y) {
    dsVector2f dir = dsVector2f(GLfloat(center[0] - eye[0]), GLfloat(center[1] - eye[1]));
    dir.normalise();
    dsVector2f left = dir.turnLeft();

    switch (key) {
    case 'a':
    case 'A':
        center[0] += left.x * viewMoveSpeed;
        center[1] += left.y * viewMoveSpeed;
        break;

    case 'w':
    case 'W':
        center[0] += dir.x * viewMoveSpeed;
        center[1] += dir.y * viewMoveSpeed;
        break;

    case 'd':
    case 'D':
        center[0] -= left.x * viewMoveSpeed;
        center[1] -= left.y * viewMoveSpeed;
        break;

    case 's':
    case 'S':
        center[0] -= dir.x * viewMoveSpeed;
        center[1] -= dir.y * viewMoveSpeed;
        break;

    default:
        break;
    }
}

//效果不理想
//void dsPassiveMonitionFunc(int x,int y){
//  extern int width;
//  extern int height;
//  if(x<width/20){
//      center[0]-=viewMoveSpeed;
//  }else if(x>width*19/20){
//      center[0]+=viewMoveSpeed;
//  }
//}


// 测试用的函数，画一个直角坐标系
void dsShowAxes() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(60, 1, 2, 1000);
    glMatrixMode(GL_MODELVIEW);

    glViewport(0, 0, 100, 100);
    //glPushMatrix();
    //glLoadIdentity();
    glTranslated(center[0], center[1], center[2]);

    //glutSolidCube(1);
    glLineWidth(1.0);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
    {
        glColor3d(1.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(axeLength, 0.0, 0.0);

        glColor3d(0.0, 1.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, axeLength, 0.0);

        glColor3d(0.0, 0.0, 1.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, axeLength);
    }
    glEnd();
    //glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

void fillRectange2D(int x, int y, int width, int length) {
    int height = -10;
    glBegin(GL_QUADS);
    {
        glVertex3d(x, y, height);
        glVertex3d(x + width, y, height);
        glVertex3d(x + width, y + length, height);
        glVertex3d(x, y + length, height);
    }
    glEnd();
}

void drawRectange2D(int x, int y, int width, int length) {
    int height = -10;
    glBegin(GL_LINE_LOOP);
    {
        glVertex3d(x, y, height);
        glVertex3d(x + width, y, height);
        glVertex3d(x + width, y + length, height);
        glVertex3d(x, y + length, height);
    }
    glEnd();
}
